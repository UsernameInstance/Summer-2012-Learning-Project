#ifndef TILE_H
#define TILE_H

#include <SDL.h>

class Tile {
	public:
		static int InfertilityLength; //in seconds
		static int Size; //tile image size pixels
		Tile(int TileID = 0, int TypeID = 0);

		int TileID;  //determines where it sits in the spritesheet
		int TypeID; //determines any attributes
		float StaticFrictionC, KineticFrictionC; //Coefficient;
		bool Fertile; //determines whether or not a plant can grow.  Resets 10 seconds of unoccupation.
		int InfertileTimestamp;  //Time at which became infertile. 0 => Never.

		struct RGBColor8 {
			Uint8 R;
			Uint8 G;
			Uint8 B;
			//Uint32 HexColor;  //Hexcolor = (R << 16) + (G << 8) + B
		};

		RGBColor8 Color;

		enum {
			TILE_TYPE_NONE,
			TILE_TYPE_DIRT,
			TILE_TYPE_MUD,
			TILE_TYPE_WATER
		};

		
};

#endif