#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h"

class MainMenu: public Menu {
	public:
		void OnInit(int X, int Y);
		void OnEvent(SDL_Event* Event);

		void OnLButtonUp(int mX, int mY);
		void OnRender(SDL_Surface* Surf_Display);

		enum {
			BUTTON_TYPE_NEW,
			BUTTON_TYPE_LOAD,
			BUTTON_TYPE_OPTIONS
		};
};

#endif