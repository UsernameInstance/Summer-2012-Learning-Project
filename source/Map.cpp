#include "Map.h"

bool Map::InstanceFlag = false;
Map* Map::theMap = NULL;

void Map::MakeInstance() {
	if(!InstanceFlag) {
		theMap = new Map();
		InstanceFlag = true;
	}
}

Map::Map() {
	Surf_Tileset = NULL;
	TilesetWidth = 48;
}

Map::~Map() {
	SDL_FreeSurface(Surf_Tileset);
}

void Map::OnNew(int Width, int Height) {
	this->Width = Width;
	this->Height = Height;
	
	TileList.clear();

	if(Surf_Tileset != NULL) SDL_FreeSurface(Surf_Tileset);
	Surf_Tileset = Surface::OnLoad("../Debug/gfx/tilesets/1.png");  //might add multiple tilesets later
	
	int RandomNum;
	
	for(int i = 0; i  < (Width / Tile::Size)*(Height / Tile::Size); i++) { //16 is tilesize for now, in pixels
		RandomNum = 1 + (rand() % 1000);
		if(RandomNum <= 999) TileList.push_back(Tile(Tile::TILE_TYPE_DIRT - 1, Tile::TILE_TYPE_DIRT));
		else if (RandomNum <= 999) TileList.push_back(Tile(Tile::TILE_TYPE_MUD - 1, Tile::TILE_TYPE_MUD)); 
		else TileList.push_back(Tile(Tile::TILE_TYPE_WATER - 1, Tile::TILE_TYPE_WATER));
	}
}

bool Map::OnLoad(char* File) {
	//for loading pre existing maps

	return true;
}

//bool Map::OnSave(char* File) { fill in later }

void Map::OnRender(SDL_Surface* Surf_Display, int MapX, int MapY) {
	int CamTilesPerRow = (*Camera::theCamera->Width) / Tile::Size, CamTilesPerCol = (*Camera::theCamera->Height) / Tile::Size; 
	int TilesPerRow = Width / Tile::Size, TilesPerCol = Height / Tile::Size; //magic number 16 is tilesize
	int TileX, TileY; //X,Y coordinates of tile in pixels, relative to Map origin
	int TilesetX, TilesetY; //X,Y coordiantes of tile graphic sprite on spritesheet, in pixels.
	int ID; //Will Hold TileID
	//MapX, MapY, relative to camera origin.
	int ntX = -MapX / Tile::Size, ntY = -MapY / Tile::Size; //camera coordinates in tile units rounded down;
	for(int Y = ntY; Y < ntY + CamTilesPerCol; Y++) {
		for(int X = ntX ; X < ntX + CamTilesPerRow; X++) {
			ID = X + TilesPerRow * Y;
			if(ID < TileList.size()) {
				TileX = X * Tile::Size;
				TileY = Y * Tile::Size;
				TilesetX = (((TileList[ID].TileID)) % (TilesetWidth / Tile::Size)) * Tile::Size;
				TilesetY = ((TileList[ID].TileID) / (TilesetWidth / Tile::Size)) * Tile::Size;  //temp

				Surface::OnDraw(Surf_Display, Surf_Tileset, TileX, TileY, TilesetX, TilesetY, Tile::Size, Tile::Size);
			}
		}
	}
}

Tile* Map::GetTile(int X, int Y) {
	int ID = 0;
	ID = X/Tile::Size;
	ID = ID + (Y/Tile::Size)*(Width/Tile::Size);

	if(ID < 0 || ID > TileList.size()) return 0;

	return &TileList[ID];
}