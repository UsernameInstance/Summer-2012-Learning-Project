#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <vector>
#include "Button.h"
#include "Event.h"
#include "AppStateManager.h"

class Menu: public Event {
	public:
		bool Running;
	
		int X;
		int Y;
		int Width;
		int Height;
		SDL_Surface* Surf_Background;

		std::vector<Button*> ButtonList;

		Menu(int X = 0, int Y = 0, int Width = 640, int Height = 480);
		~Menu();


		void AddButton(char* File, int TypeID, int X, int Y, int Width, int Height);
		
		//functions should be overwritten in derived classes.
		virtual void OnInit();
		virtual void OnEvent(SDL_Event* Event);
		virtual void OnLButtonDown(int mX, int mY);
		virtual void OnLButtonUp(int mX, int mY);
		virtual void OnRender(SDL_Surface* Surf_Display);

		//button types go here

		enum {  //App status list
			MAIN_MENU,
			OPTIONS_MENU,
			LOAD_MENU,
			PAUSE_MENU,
			WORLD_SETTINGS_MENU,
			WORLD_RUNNING
		};
};

#endif