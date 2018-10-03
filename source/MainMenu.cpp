#include "MainMenu.h"

void MainMenu::OnInit(int X, int Y) {
	this->X = X;
	this->Y = Y;

	Surf_Background = Surface::OnLoad("../Debug/gfx/menubgs/mainmenubg.png");
	AddButton("../Debug/gfx/buttons/newbutton.bmp", BUTTON_TYPE_NEW, this->X + Width / 2 - 80, this->Y + Height / 3, 160, 60); //temp
}

void MainMenu::OnEvent(SDL_Event* Event) {
	Event::OnEvent(Event);
}

void MainMenu::OnLButtonUp(int mX, int mY) {
	if(ButtonList.size() != 0) {
		for(int i = 0; i < (int) ButtonList.size(); i++) {
			if(mX >= (ButtonList[i]->X) && mX <= (ButtonList[i]->X + ButtonList[i]->Width) 
			&& (mY >= ButtonList[i]->Y) && mY <= (ButtonList[i]->Y + ButtonList[i]->Height)) {
				if(!ButtonList[i]->OnClick()) {
					switch (ButtonList[i]->TypeID) {
						case BUTTON_TYPE_NEW:
							AppStateManager::theAppStateManager->AppState = WORLD_SETTINGS_MENU;
							return;
						case BUTTON_TYPE_LOAD:
							AppStateManager::theAppStateManager->AppState = LOAD_MENU;
							return;
						case BUTTON_TYPE_OPTIONS:
							AppStateManager::theAppStateManager->AppState = OPTIONS_MENU;
							return;		
					}
				}
			}
			ButtonList[i]->Clicked = false;
		}
	}
}

void MainMenu::OnRender(SDL_Surface* Surf_Display) {
	SDL_FillRect(Surf_Display, NULL, 0);
	Surface::OnDraw(Surf_Display, Surf_Background, X, Y);
	for(int i = 0; i < (int) ButtonList.size(); i++) {
		if(ButtonList[i]->Clicked) Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 160, 0, ButtonList[i]->Width, ButtonList[i]->Height);
		else Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 0, 0, ButtonList[i]->Width, ButtonList[i]->Height);
	}
}
