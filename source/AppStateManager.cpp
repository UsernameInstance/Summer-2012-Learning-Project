#include "AppStateManager.h"

bool AppStateManager::InstanceFlag = false;
AppStateManager* AppStateManager::theAppStateManager = NULL;

void AppStateManager::MakeInstance() {
	if(!InstanceFlag) {
		theAppStateManager = new AppStateManager();
		InstanceFlag = true;
	}
}

AppStateManager::AppStateManager() {
	AppState = MAIN_MENU;
}

AppStateManager::~AppStateManager() {
	InstanceFlag = false;
}