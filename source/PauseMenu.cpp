#include "PauseMenu.h"

void PauseMenu::OnInit(int X, int Y) {
	this->X = X;
	this->Y = Y;

	Surf_Background = Surface::OnLoad("../Debug/gfx/menubgs/pausemenubg.bmp");
	AddButton("../Debug/gfx/buttons/resumebutton.bmp", BUTTON_TYPE_RESUME, this->X + Width / 2 - 95, this->Y + Height / 3, 190, 60); 
	AddButton("../Debug/gfx/buttons/quitbutton.bmp", BUTTON_TYPE_QUIT, this->X + Width / 2 - 80, this->Y + Height / 3 + 180, 160, 60); 
}

void PauseMenu::OnEvent(SDL_Event* Event) {
	Event::OnEvent(Event);
}

void PauseMenu::OnLButtonUp(int mX, int mY) {
	if(ButtonList.size() != 0) {
		for(int i = 0; i < (int) ButtonList.size(); i++) {
			if(mX >= (ButtonList[i]->X) && mX <= (ButtonList[i]->X + ButtonList[i]->Width) 
			&& (mY >= ButtonList[i]->Y) && mY <= (ButtonList[i]->Y + ButtonList[i]->Height)) {
				if(!ButtonList[i]->OnClick()) {
					switch (ButtonList[i]->TypeID) {
						case BUTTON_TYPE_RESUME:
							AppStateManager::theAppStateManager->AppState = WORLD_RUNNING;
							return;
						case BUTTON_TYPE_QUIT:
							delete World::theWorld;
							AppStateManager::theAppStateManager->AppState = MAIN_MENU;
							return;
					}
				}
			}
			ButtonList[i]->Clicked = false;
		}
	}
}

void PauseMenu::OnRender(SDL_Surface* Surf_Display) {
	Surface::OnDraw(Surf_Display, Surf_Background, X, Y);
	for(int i = 0; i < (int) ButtonList.size(); i++) {
		if(ButtonList[i]->Clicked) Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, ButtonList[i]->Width, 0, ButtonList[i]->Width, ButtonList[i]->Height);
		else Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 0, 0, ButtonList[i]->Width, ButtonList[i]->Height);
	}
}
