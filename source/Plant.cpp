#include "Plant.h"

//precache shared plant images to improve performance.
std::vector<SDL_Surface*> Plant::PlantFrameList; //shared between all plant entities
Plant::RGBColor8 Plant::Color;

std::vector<Plant*> Plant::PlantList;

void Plant::OnInit() {
	SDL_Surface* Surf_Temp = Surface::OnLoad("../Debug/gfx/PlantTest.bmp");
	SDL_Surface* Surf_Temp2;
	SDL_Surface* Surf_Temp3;

	for(int i = 0; i <= 10; i++) {
		Color.R = (Uint8)(-i*15.1 + 185);
		Color.G = (Uint8)(i*5.5 + 122);
		Color.B = (Uint8)(-i*1.0 + 87);

		if(Surf_Temp != NULL) {
			Surf_Temp2 = Surface::ScaleSurface(Surf_Temp, Surf_Temp->w, Surf_Temp->h);
			Surface::SetSurfColor(Surf_Temp2, SDL_MapRGB(Surf_Temp2->format, 100, 100, 100), SDL_MapRGB(Surf_Temp2->format, Color.R, Color.G, Color.B));
			Surf_Temp3 = Surface::ScaleSurface(Surf_Temp2, 16, 16);
			SDL_FreeSurface(Surf_Temp2);
			Surface::Transparent(Surf_Temp3, 255, 0, 255);

			PlantFrameList.push_back(Surf_Temp3);
		}
	}

	SDL_FreeSurface(Surf_Temp);
}

Plant::Plant(float X, float Y): Entity(X, Y, Entity::ENTITY_TYPE_PLANT) {
	MaxEnergy = 500;
	OldEnergy = CurrentEnergy = 250;
	Radius = 1; //meter
	Lifespan = 180; //seconds at 1x WorldSpeed 
	Age = 0;
	LReAtTime = SDL_GetTicks();
	/*
	Color.R = (Uint8)(-151 * (CurrentEnergy/MaxEnergy) + 185);
	Color.G = (Uint8)(55 * (CurrentEnergy/MaxEnergy) + 122);
	Color.B = (Uint8)(-11 * (CurrentEnergy/MaxEnergy) + 87);
	if(Color.R < 34) Color.R = 34;
	if(Color.G < 122) Color.G = 122;
	if(Color.B < 76) Color.B = 76;
	if(Color.R > 185) Color.R = 185;
	if(Color.G > 177) Color.G = 177;
	if(Color.B > 87) Color.B = 87;

	Surf_Basis = Surface::RotateSurface(Surf_Entity, 0);
	Surface::SetSurfColor(Surf_Entity, SDL_MapRGB(Surf_Entity->format, 100, 100, 100), SDL_MapRGB(Surf_Entity->format, Color.R, Color.G, Color.B));
	Surface::Transparent(Surf_Entity, 255, 0, 255);
	*/



}

Plant::~Plant() { }

void Plant::OnLoop() {
	OldEnergy = CurrentEnergy;

	Tile* tempTile = Map::theMap->GetTile((int)(X), (int)(Y));
	if((tempTile->TypeID == Tile::TILE_TYPE_DIRT || tempTile->TypeID == Tile::TILE_TYPE_MUD)) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;

	//give energy bonus if adjacent tile is water;
	int ID = 0;
	ID = (int)X/Tile::Size;
	ID = (int)(ID + (Y/Tile::Size)*(Map::theMap->Width/Tile::Size));

	if(ID + 1 < Map::theMap->TileList.size()) if(Map::theMap->TileList[ID+1].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
	if(0 <= ID - 1) if(Map::theMap->TileList[ID-1].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
	if(0 <= ID - (Map::theMap->Width/Tile::Size)) {
		if(Map::theMap->TileList[ID - (Map::theMap->Width/Tile::Size)].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
		if(Map::theMap->TileList[ID - (Map::theMap->Width/Tile::Size) + 1].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
	}
	if(0 <= ID - (Map::theMap->Width/Tile::Size) - 1) if(Map::theMap->TileList[ID - (Map::theMap->Width/Tile::Size) - 1].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
	if(ID + (Map::theMap->Width/Tile::Size) - 1 < Map::theMap->TileList.size()) if(Map::theMap->TileList[ID + (Map::theMap->Width/Tile::Size) - 1].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
	if(ID + (Map::theMap->Width/Tile::Size) < Map::theMap->TileList.size()) if(Map::theMap->TileList[ID + (Map::theMap->Width/Tile::Size)].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;
	if(ID + (Map::theMap->Width/Tile::Size) +1 < Map::theMap->TileList.size()) if(Map::theMap->TileList[ID + (Map::theMap->Width/Tile::Size) + 1].TypeID == Tile::TILE_TYPE_WATER) CurrentEnergy += 10*FPS::FPSControl->SpeedFactor;

	if(CurrentEnergy > MaxEnergy) CurrentEnergy = MaxEnergy;
	
	if(CurrentEnergy == MaxEnergy  && PlantList.size() < WorldSettings::MaxNumPlants && LReAtTime + 500 < SDL_GetTicks()) {
			LReAtTime = SDL_GetTicks();

			if(!(rand() % 2)) {
				int Rand = rand() % 8;
				OnReproduce(Rand);
			}
		}

	Age = (float)(SDL_GetTicks() - CreationTime)/1000.0;
	if(Age >= Lifespan) CurrentEnergy = 0;
	else if(Age > (0.75)*Lifespan) CurrentEnergy = OldEnergy - (MaxEnergy / 100.0) * FPS::FPSControl->SpeedFactor;

	if(CurrentEnergy <= 0) {
		CurrentEnergy = 0;
		Dead = true;
		Tile* tempTileP = Map::theMap->GetTile(X, Y);
		tempTileP->Fertile = false;
		tempTileP->InfertileTimestamp = SDL_GetTicks();
	}

	int i = (int)(CurrentEnergy/100.0) % (int)((MaxEnergy/100.0) + 1);
	Color.R = (Uint8)(-i*15.1 + 185.0);
	Color.G = (Uint8)(i*5.5 + 122.0);
	Color.B = (Uint8)(-i*1.0 + 87.0);
}

void Plant::OnRender(SDL_Surface* Surf_Display) {
	int i = (int)((CurrentEnergy/MaxEnergy)*10.0);
	if(PlantFrameList.size() > 0 && i <= 10) Surface::OnDraw(Surf_Display, PlantFrameList[i], (int)X - Camera::theCamera->X, int(Y) - Camera::theCamera->Y);

	/*
	Color.R = (Uint8)(-151 * (CurrentEnergy/MaxEnergy) + 185);
	Color.G = (Uint8)(-55 * (CurrentEnergy/MaxEnergy) + 122);
	Color.B = (Uint8)(-11 * (CurrentEnergy/MaxEnergy) + 87);

	if(Color.R < 34) Color.R = 34;
	if(Color.G < 122) Color.G = 122;
	if(Color.B < 76) Color.B = 76;
	if(Color.R > 185) Color.R = 185;
	if(Color.G > 177) Color.G = 177;
	if(Color.B > 87) Color.B = 87;
	
	Surf_Entity = Surface::RotateSurface(Surf_Basis, 0);
		
	Surface::SetSurfColor(Surf_Entity, SDL_MapRGB(Surf_Entity->format, 100, 100, 100), SDL_MapRGB(Surf_Entity->format, Color.R, Color.G, Color.B));
	SDL_Surface* Temporary = Surface::ScaleSurface(Surf_Entity, 16, 16);
	SDL_FreeSurface(Surf_Entity);
	Surf_Entity = Temporary;
	Surface::Transparent(Surf_Entity, 255, 0, 255);
	Surface::OnDraw(Surf_Display, Surf_Entity, X, Y);
	*/
}

bool Plant::OnReproduce(int ID) {
	if(PlantList.size() >= WorldSettings::MaxNumPlants) return false;

	Tile* tempTileP;
	Plant* tempPlant;
	bool ValidPos = true;

	switch(ID) {
		case 0:
			if(PosValidMap(X - Tile::Size, Y - Tile::Size)) {
				tempTileP = Map::theMap->GetTile(X - Tile::Size, Y - Tile::Size);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X - Tile::Size && PlantList[i]->Y == Y - Tile::Size) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X - Tile::Size, Y - Tile::Size);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 1:
			if(PosValidMap(X, Y - Tile::Size)) {
				tempTileP = Map::theMap->GetTile(X, Y - Tile::Size);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER  && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X && PlantList[i]->Y == Y - Tile::Size) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X, Y - Tile::Size);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 2:
			if(PosValidMap(X + Tile::Size, Y - Tile::Size)) {
				tempTileP = Map::theMap->GetTile(X + Tile::Size, Y - Tile::Size);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X + Tile::Size && PlantList[i]->Y == Y - Tile::Size) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X + Tile::Size, Y - Tile::Size);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 3:
			if(PosValidMap(X - Tile::Size, Y)) {
				tempTileP = Map::theMap->GetTile(X - Tile::Size, Y);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X - Tile::Size && PlantList[i]->Y == Y) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X - Tile::Size, Y);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 4:
			if(PosValidMap(X + Tile::Size, Y)) {
				tempTileP = Map::theMap->GetTile(X + Tile::Size, Y);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X + Tile::Size && PlantList[i]->Y == Y) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X + Tile::Size, Y);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 5:
			if(PosValidMap(X - Tile::Size, Y + Tile::Size)) {
				tempTileP = Map::theMap->GetTile(X - Tile::Size, Y + Tile::Size);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X - Tile::Size && PlantList[i]->Y == Y + Tile::Size) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X - Tile::Size, Y + Tile::Size);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 6:
			if(PosValidMap(X, Y + Tile::Size)) {
				tempTileP = Map::theMap->GetTile(X, Y + Tile::Size);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X && PlantList[i]->Y == Y + Tile::Size) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X, Y + Tile::Size);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		case 7:
			if(PosValidMap(X + Tile::Size, Y + Tile::Size)) {
				tempTileP = Map::theMap->GetTile(X + Tile::Size, Y + Tile::Size);
				if(tempTileP != 0) {
					if(tempTileP->TypeID != Tile::TILE_TYPE_WATER && (tempTileP->Fertile || (!tempTileP->Fertile && SDL_GetTicks() - tempTileP->InfertileTimestamp > Tile::InfertilityLength * 1000)) ) {
						ValidPos = true;
						for(int i = 0; i < PlantList.size(); i++) {
							if(PlantList[i]->X == X + Tile::Size && PlantList[i]->Y == Y + Tile::Size) ValidPos = false; 
						}
						if(ValidPos) {
						tempPlant = new Plant(X + Tile::Size, Y + Tile::Size);
						EntityList.push_back(tempPlant);
						PlantList.push_back(tempPlant);
						if(!tempTileP->Fertile) tempTileP->Fertile = true; //set infertility timestamp on death
						}
					}
				}
			}
			break;

		default:
			ValidPos = false;
	}

	if(ValidPos) {
		CurrentEnergy -= MaxEnergy/2.0;
		return true;
	}

	return false;
}

bool Plant::OnCollision(Entity* TargetEntity, bool BodyCol) {
	if(TargetEntity->Type == Entity::ENTITY_TYPE_ANIMAL) {
		CurrentEnergy -= 30*FPS::FPSControl->SpeedFactor;
		if(CurrentEnergy < 0) CurrentEnergy = 0;
	}
	return false; //dont want it to calculate twice, although might need to change this to true.
}