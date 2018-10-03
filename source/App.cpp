#include "App.h"

bool App::InstanceFlag = false;
App* App::theApp = NULL;

void App::MakeInstance() {
	if(!InstanceFlag) {
		theApp = new App();
		InstanceFlag = true;
	}
}

App::App(int Width, int Height) {
	Surf_Display = NULL;

	Running = true;
	this->Width = Width;
	this->Height = Height;
}

App::~App() {
	InstanceFlag = false;
}

bool App::OnInit() {
	//SDL_INIT_EVERYTHING initializes SDL runtime support? and allows access to its functions. 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

	//Initialize display.  Takes 4 parameters.  Parameters in order left to right:
	//Window Height, window height, bit resolution, display flags.  Height and height in pixels.  Bit resolution designates bits per pixel.
	//SDL_HWSURFACE tlls sdl to use hardware memory for storing images and such.  SDL_DOUBLEBUF tells SDL to use double buffering (prevents screen flickering).
	//SDL_FULLSCREEN sets window to fullscreen.  | seems to be an 'and'..?
	if((Surf_Display = SDL_SetVideoMode(Width, Height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) return false;

	//Enables or disables the keyboard repeat rate.  1st parameter is delay and 2nd parameter is repeat rate, in milliseconds.
	SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

	FPS::MakeInstance();

	AppStateManager::MakeInstance();
	Camera::MakeInstance();
	Camera::theCamera->Width = &Width;
	Camera::theCamera->Height = &Height;
	World::MakeInstance();
	Map::MakeInstance();

	theMainMenu.OnInit((Width - theMainMenu.Width) / 2, (Height - theMainMenu.Height) / 2);
	theWorldSettingsMenu.OnInit((Width - theMainMenu.Width) / 2, (Height - theMainMenu.Height) / 2);
	thePauseMenu.OnInit((Width - thePauseMenu.Width) / 2, (Height - thePauseMenu.Height) / 2);

	return true;
}

int App::OnRun() {
	if(OnInit() == false) return -1;

	SDL_Event Event;

	while(Running) {
		while(SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}

		OnLoop();
		OnRender();
	}

	OnCleanup();

	return 0;
}

void App::OnEvent(SDL_Event* Event) {
	Event::OnEvent(Event);
	switch (AppStateManager::theAppStateManager->AppState) {
		case MAIN_MENU:
			theMainMenu.OnEvent(Event);
			break;
		case WORLD_SETTINGS_MENU:
			theWorldSettingsMenu.OnEvent(Event);
			break;
		case WORLD_RUNNING:
			World::theWorld->OnEvent(Event);
			break;
		case PAUSE_MENU:
			thePauseMenu.OnEvent(Event);
			break;
	}
}

void App::OnExit() {
	Running = false;
}

void App::OnLoop() {
	switch(AppStateManager::theAppStateManager->AppState) {
		case WORLD_RUNNING:
		World::theWorld->OnLoop();
	}

	FPS::FPSControl->OnLoop();
}

void App::OnRender() {
	//SDL_FillRect(Surf_Display, NULL, 0);

	switch(AppStateManager::theAppStateManager->AppState) {
		case MAIN_MENU:
			theMainMenu.OnRender(Surf_Display);
			break;
		case WORLD_SETTINGS_MENU:
			theWorldSettingsMenu.OnRender(Surf_Display);
			break;
		case WORLD_RUNNING:
			World::theWorld->OnRender(Surf_Display);
			break;
		case PAUSE_MENU:
			thePauseMenu.OnRender(Surf_Display);
			break;
	}

	SDL_Flip(Surf_Display);
}

void App::OnCleanup() {
	SDL_FreeSurface(Surf_Display);

	SDL_Quit();
}

int main(int argc, char* argv[]) {
	srand((unsigned)time(NULL));

	App::MakeInstance();

	return App::theApp->OnRun();
}