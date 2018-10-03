#include "Menu.h"


Menu::Menu(int X, int Y, int Width, int Height) {
	this->X = X;
	this->Y = Y;
	this->Width = Width;
	this->Height = Height;
	Running = true;
}

Menu::~Menu() {
	if(Surf_Background != NULL) SDL_FreeSurface(Surf_Background);

	for(int i = 0; i < (int) ButtonList.size(); i++) {
		delete ButtonList[i];
	}
	ButtonList.clear();
}



void Menu::AddButton(char* File, int TypeID, int X, int Y, int Width, int Height) {
	Button* tempButton = new Button(TypeID, X, Y, Width, Height);
	ButtonList.push_back(tempButton);
	ButtonList.back()->Surf_Button = Surface::OnLoad(File);
}

void Menu::OnInit() {
	Surf_Background = Surface::OnLoad("../Debug/gfx/menubgs/mainmenubg.png");
	//Add buttons here.
}

void Menu::OnEvent(SDL_Event* Event) {
	//pure virtual
}

void Menu::OnLButtonDown(int mX, int mY) {
	if(ButtonList.size() != 0) {
		for(int i = 0; i < (int) ButtonList.size(); i++) {
			if(mX >= (ButtonList[i]->X) && mX <= (ButtonList[i]->X + ButtonList[i]->Width) 
			&& (mY >= ButtonList[i]->Y) && mY <= (ButtonList[i]->Y + ButtonList[i]->Height)) if(!ButtonList[i]->OnClick());
		}
	}
}

void Menu::OnLButtonUp(int mX, int mY) {
	if(ButtonList.size() != 0) {
		for(int i = 0; i < (int) ButtonList.size(); i++) {
			if(mX >= (ButtonList[i]->X) && mX <= (ButtonList[i]->X + ButtonList[i]->Width) 
			&& (mY >= ButtonList[i]->Y) && mY <= (ButtonList[i]->Y + ButtonList[i]->Height)) {
				if(!ButtonList[i]->OnClick()) {
					switch (ButtonList[i]->TypeID) {
						 //button function goes here	
					}
				}
			}
		}
	}
	AppStateManager::theAppStateManager->AppState = MAIN_MENU;
}

void Menu::OnRender(SDL_Surface* Surf_Display) {
	Surface::OnDraw(Surf_Display, Surf_Background, X, Y);
	for(int i = 0; i < (int) ButtonList.size(); i++) {
		if(ButtonList[i]->Clicked) Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 160, 0, ButtonList[i]->Width, ButtonList[i]->Height);
		else Surface::OnDraw(Surf_Display, ButtonList[i]->Surf_Button, ButtonList[i]->X, ButtonList[i]->Y, 0, 0, ButtonList[i]->Width, ButtonList[i]->Height);
	}
}
