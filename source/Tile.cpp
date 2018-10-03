#include "Tile.h"

int Tile::InfertilityLength = 30;
int Tile::Size = 16;

Tile::Tile(int TileID, int TypeID) {
	this->TileID = TileID;
	this->TypeID = TypeID;
	InfertileTimestamp = 0;
	switch (TypeID) {
	case TILE_TYPE_NONE:
		StaticFrictionC = 0;
		KineticFrictionC = 0;
		Fertile = false;
		break;
	case TILE_TYPE_DIRT:
		StaticFrictionC = 0.04;
		KineticFrictionC = 0.02;
		Fertile = true;
		Color.R = 144;
		Color.G = 102;
		Color.B = 72;
		break;
	case TILE_TYPE_WATER:
		StaticFrictionC = 0.06;
		KineticFrictionC = 0.03;
		Fertile = false;
		Color.R = 63;
		Color.G = 72;
		Color.B = 204;
		break;
	case TILE_TYPE_MUD:
		StaticFrictionC = 0.08;
		KineticFrictionC = 0.04;
		Fertile = true;
		Color.R = 92;
		Color.G = 68;
		Color.B = 73;
		break;
	}
}