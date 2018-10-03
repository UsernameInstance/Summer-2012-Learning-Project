#ifndef APP_H
#define APP_H

#include <SDL.h>
#include "MainMenu.h"
#include "WorldSettingsMenu.h"
#include "PauseMenu.h"
#include "FPS.h"
#include "World.h"
#include <stdlib.h>

class App: public Event {
	private:
		static bool InstanceFlag;
		App(int Width = 1280, int Height = 960);

	public:
		static App* theApp;  //The static App pointer
		static void MakeInstance(); //implement a singleton
		~App();

		MainMenu theMainMenu;
		WorldSettingsMenu theWorldSettingsMenu;
		PauseMenu thePauseMenu;

		int Width; //display length
		int Height; //display width

		bool Running;

		SDL_Surface* Surf_Display;

		int OnRun();
		bool OnInit();  //initialize

		void OnEvent(SDL_Event* Event); //Routes events to memberclasses depending on AppState
		void OnLoop();
		void OnRender();

		void OnCleanup();
		void OnExit();

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