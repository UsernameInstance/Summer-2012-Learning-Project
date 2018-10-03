#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <vector>
#include <cstdlib>  //temporary.  Will implement own random number generator later.
#include <ctime>

#include "Tile.h"
#include "Surface.h"

#include "Camera.h"

class Map {
	private:
		static bool InstanceFlag;
		Map();

	public:
		static void MakeInstance();
		static Map* theMap;
		int Width; //get from WorldSize
		int Height;

		~Map();

		std::vector<Tile> TileList;

		SDL_Surface* Surf_Tileset;

		int TilesetWidth;  //width of Tileset spritesheet, in pixels.

		void OnNew(int Width, int Height); //Generates a random map of specified size

		//bool OnSave(char* File);  implement later

		bool OnLoad(char* File);

		void OnRender(SDL_Surface* Surf_Display, int MapX, int MapY);

		Tile* GetTile(int X, int Y);
};

#endif