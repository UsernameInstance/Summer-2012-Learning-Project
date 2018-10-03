#ifndef WORLD_SETTINGS_MENU_H
#define WORLD_SETTINGS_MENU_H

#include "Menu.h"
#include "World.h"
#include "WorldSettings.h"

class WorldSettingsMenu: public Menu {
	public:
		void OnInit(int X, int Y);
		void OnEvent(SDL_Event* Event);

		void OnLButtonUp(int mX, int mY);
		void OnRender(SDL_Surface* Surf_Display);

		enum {
			BUTTON_TYPE_BACK,
			BUTTON_TYPE_START,
		};
};

#endif