#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Menu.h"
#include "World.h"

class PauseMenu: public Menu {
	public:
		void OnInit(int X, int Y);
		void OnEvent(SDL_Event* Event);

		void OnLButtonUp(int mX, int mY);
		void OnRender(SDL_Surface* Surf_Display);

		enum {
			BUTTON_TYPE_RESUME,
			BUTTON_TYPE_QUIT
		};
};

#endif