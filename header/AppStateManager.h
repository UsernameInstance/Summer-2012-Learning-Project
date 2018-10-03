#ifndef APPSTATEMANAGER_H
#define APPSTATEMANAGER_H

#include <SDL.h>

class AppStateManager {
	private:
		static bool InstanceFlag;
		AppStateManager();

	public:
		~AppStateManager();

		static AppStateManager* theAppStateManager;
		static void MakeInstance();

		int AppState;

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