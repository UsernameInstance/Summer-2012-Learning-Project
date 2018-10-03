#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>
#include <cmath>
#include "Map.h"
#include "Event.h"
#include "Animal.h"
#include "Plant.h"
#include "WorldSettings.h"
#include "AppStateManager.h"


class World: public Event {
	private:
		static bool InstanceFlag;
		World(int Width = 1024, int Height = 768, float Gravity = 9.8, float AtmDensity = 1.204, float MutationRate = 0.01, int NumAnimals = 10, int NumPlants = 10);

	public:
		static World* theWorld;
		static void MakeInstance();
		~World();

		bool SkipCol;

		void OnNew(int Width = 1024, int Height = 768, float Gravity = 9.8, float AtmDensity = 1.204, float MutationRate = 0.01, int NumAnimals = 10, int NumPlants = 10);
		void OnEvent(SDL_Event* Event);
		void OnLoop();
		void OnRender(SDL_Surface* Surf_Display);

		void OnCleanup();

		void OnLButtonDown(int mX, int mY);
		void OnLButtonUp(int mX, int mY);
		void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

		static bool GetInstanceFlag();
		static void SetInstanceFlag(bool Value);
};

#endif