#include "WorldSettingsMenu.h"
#include "App.h"

void WorldSettingsMenu::OnInit(int X, int Y) {
	this->X = X;
	this->Y = Y;

	Surf_Background = Surface::OnLoad("../Debug/gfx/menubgs/worldsettingsmenubg.png");
	AddButton("../Debug/gfx/buttons/backbutton.png", BUTTON_TYPE_BACK, this->X + Width / 2 - 80, this->Y + Height / 3 + 180, 160, 60); 
	AddButton("../Debug/gfx/buttons/startbutton.png", BUTTON_TYPE_START, this->X + Width / 2 - 80, this->Y + Height / 3, 160, 60);
}

void WorldSettingsMenu::OnEvent(SDL_Event* Event) {
	Event::OnEvent(Event);
}


void WorldSettingsMenu::OnLButtonUp(int mX, int mY) {
	if(ButtonList.size() != 0) {
		for(int i = 0; i < (int) ButtonList.size(); i++) {
			if(mX >= (ButtonList[i]->X) && mX <= (ButtonList[i]->X + ButtonList[i]->Width) 
			&& (mY >= ButtonList[i]->Y) && mY <= (ButtonList[i]->Y + ButtonList[i]->Height)) {
				if(!ButtonList[i]->OnClick()) {
					switch (ButtonList[i]->TypeID) {
						case BUTTON_TYPE_BACK:
							AppStateManager::theAppStateManager->AppState = MAIN_MENU;
							return;
						case BUTTON_TYPE_START:
							AppStateManager::theAppStateManager->AppState = WORLD_RUNNING;
							if(!World::GetInstanceFlag()) World::MakeInstance();
							World::theWorld->OnNew(App::theApp->Width, App::theApp->Height);  //should probably make the world singleton
							return;
					}
				}
			}
			ButtonList[i]->Clicked = false;
		}
	}
}

void WorldSettingsMenu::OnRender(SDL_Surface* Surf_Display) {
	SDL_FillRect(Surf_Display, NULL, 0);
	Surface::OnDraw(Surf_Display, Surf_Background, X, Y);
	for(int i = 0; i < (int) ButtonList.size(); i++) {
		if(ButtonList[i]->Clicked) Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 160, 0, ButtonList[i]->Width, ButtonList[i]->Height);
		else Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 0, 0, ButtonList[i]->Width, ButtonList[i]->Height);
	}
}
