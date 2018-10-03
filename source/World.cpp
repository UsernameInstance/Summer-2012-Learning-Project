#include "World.h"

bool World::InstanceFlag = false;
World* World::theWorld = NULL;

void World::MakeInstance() {
	if(!InstanceFlag) {
		theWorld = new World();
		InstanceFlag = true;
	}
}

World::World(int Width, int Height, float Gravity, float AtmDensity, float MutationRate, int NumAnimals, int NumPlants) {
	WorldSettings::Width = Width;
	WorldSettings::Height = Height;
	WorldSettings::Gravity = Gravity;
	WorldSettings::AtmDensity = AtmDensity;
	WorldSettings::MutationRate = MutationRate;
	WorldSettings::NumAnimals = NumAnimals;
	WorldSettings::NumPlants = NumPlants;
	WorldSettings::WorldSpeed = 1;
	SkipCol = false;
}

World::~World() {
	//free dynamically allocated memory, such as vectors, surfaces, misc pointers.
	if(Entity::EntityList.size() > 0) for(int i = 0; i < Entity::EntityList.size(); i ++) delete Entity::EntityList[i];
	//if(Animal::AnimalList.size() > 0) for(int i = 0; i < Animal::AnimalList.size(); i ++) delete Animal::AnimalList[i]; //the size is greater than zero but entitylist deleted everything here already.
	//if(Plant::PlantList.size() > 0) for(int i = 0; i < Plant::PlantList.size(); i ++) delete Plant::PlantList[i];
	if(Plant::PlantFrameList.size() > 0) for(int i = 0; i < Plant::PlantFrameList.size(); i ++) SDL_FreeSurface(Plant::PlantFrameList[i]);

	Entity::EntityList.clear();
	Animal::AnimalList.clear();
	Plant::PlantList.clear();
	Plant::PlantFrameList.clear();

	InstanceFlag = false;
}

bool World::GetInstanceFlag() {
	return InstanceFlag;
}

void World::SetInstanceFlag(bool Value) {
	InstanceFlag = Value;
}

void World::OnNew(int Width, int Height, float Gravity, float AtmDensity, float MutationRate, int NumAnimals, int NumPlants) {
	WorldSettings::Width = Width;
	WorldSettings::Height = Height;
	WorldSettings::Gravity = Gravity;
	WorldSettings::AtmDensity = AtmDensity;
	WorldSettings::MutationRate = MutationRate;
	WorldSettings::NumAnimals = NumAnimals;
	WorldSettings::NumPlants = NumPlants;
	WorldSettings::WorldSpeed = 1;

	WorldSettings::MaxNumAnimals = 30;
	WorldSettings::MaxNumPlants = 60;
	
	Map::theMap->OnNew(Width, Height);
	/*Animal* tempAnimal = new Animal(0, 0); //there seems to be a bug in the top left of the map.  If a creature is spawned within some square with top left corner in top left of map and sides of around 50 pixels, the creature will not move, as if the 'friction' is insanely high.
	Entity::EntityList.push_back(tempAnimal); //it also stops ALL other creatures from moving translationally (i.e not rotationally).
	Animal::AnimalList.push_back(tempAnimal); //This was caused by PosValid function calling AnimalCollides function before movement.  Apparently there is a bug with AnimalCollides.
												//Solution.  The bug was caused by passing MoveX and MoveY to PosValid.  This did not check the coordinates they were moving to, but simply the displacement vector, which would obviously be near the top left.
	*/

	for(int i = 0; i < 9; i++) {
		Animal* tempAnimal = new Animal( (i % 3) * Map::theMap->Width / 3, (i/3) * Map::theMap->Height / 3, 1000, 13 + rand() % 32, 3);
		Entity::EntityList.push_back(tempAnimal); 
		Animal::AnimalList.push_back(tempAnimal);
	}

	Plant::OnInit();
	
	Plant* tempPlant = NULL;
	int PlantCounter = 0; //to set a minimum number of plants
	int ID = 0;
	int InitialNumPlants = 20;

	for(int i = 1 ; i <= InitialNumPlants; i++) {
		ID = rand() % (Map::theMap->TileList.size() / InitialNumPlants) + (Map::theMap->TileList.size() * (i - 1) / InitialNumPlants);
		if(ID - i*((Map::theMap->TileList.size()) / InitialNumPlants) >= 3) ID -= 3;
		PlantCounter = 0;
		for(int j = 0; j < 10; j++) {
			if(Map::theMap->TileList[ID].TypeID != Tile::TILE_TYPE_WATER && !(ID >= Map::theMap->TileList.size())) {
				tempPlant = new Plant((ID % (Map::theMap->Width/Tile::Size))*Tile::Size, (ID/(Map::theMap->Width/Tile::Size))*Tile::Size);
				if(tempPlant != NULL) {
					Entity::EntityList.push_back(tempPlant);
					Plant::PlantList.push_back(tempPlant);
					PlantCounter++;
					ID++;
				}
			}
			if(ID >= Map::theMap->TileList.size() || ID >= i*((Map::theMap->TileList.size()) / InitialNumPlants) || PlantCounter >=1) break;
		}
	}
}
	/*
	for(int MX = 0; MX < (int)(Map::theMap->Width/Tile::Size); MX++) {
		for(int MY = 0; MY < (int)(Map::theMap->Height/Tile::Size); MY++) {
			ID = (int)(MX + MY*(Map::theMap->Width/Tile::Size));
			if(ID < Map::theMap->TileList.size()) {
				if(Map::theMap->TileList[ID].TypeID != Tile::TILE_TYPE_WATER) {
					if(rand() % 500 == 1 && Plant::PlantList.size() < 21) {
						tempPlant = new Plant(MX*Tile::Size, MY*Tile::Size);
						if(tempPlant != NULL) {
							Entity::EntityList.push_back(tempPlant);
							Plant::PlantList.push_back(tempPlant);
							PlantCounter++;
						}
					}
				}
			}
		}
	}
	*/


void World::OnEvent(SDL_Event* Event) {
	Event::OnEvent(Event);
}

void World::OnLoop() {
	/*if(Animal::AnimalList.size() >= 3) if(Animal::AnimalList[1] != NULL && Animal::AnimalList[2] != NULL && Animal::AnimalList[0] != NULL) {
		if( sqrt((pow(Animal::AnimalList[2]->VelX, 2) + pow(Animal::AnimalList[2]->VelY, 2))) == 0 ) { 
			Animal::AnimalList[2]->VelX = 3 * ( Animal::AnimalList[1]->X - Animal::AnimalList[2]->X ) / (sqrt(pow(Animal::AnimalList[1]->X - Animal::AnimalList[2]->X, 2) + pow(Animal::AnimalList[1]->Y - Animal::AnimalList[2]->Y, 2)));
			Animal::AnimalList[2]->VelY = 3 * ( Animal::AnimalList[1]->Y - Animal::AnimalList[2]->Y ) / (sqrt(pow(Animal::AnimalList[1]->X - Animal::AnimalList[2]->X, 2) + pow(Animal::AnimalList[1]->Y - Animal::AnimalList[2]->Y, 2)));
		}
		if( sqrt((pow(Animal::AnimalList[0]->VelX, 2) + pow(Animal::AnimalList[0]->VelY, 2))) == 0 ) { 
			Animal::AnimalList[0]->VelX = 3 * ( Animal::AnimalList[2]->X - Animal::AnimalList[0]->X ) / (sqrt(pow(Animal::AnimalList[2]->X - Animal::AnimalList[0]->X, 2) + pow(Animal::AnimalList[1]->Y - Animal::AnimalList[0]->Y, 2)));
			Animal::AnimalList[0]->VelY = 3 * ( Animal::AnimalList[2]->Y - Animal::AnimalList[0]->Y ) / (sqrt(pow(Animal::AnimalList[2]->X - Animal::AnimalList[0]->X, 2) + pow(Animal::AnimalList[1]->Y - Animal::AnimalList[0]->Y, 2)));
		}
		if( sqrt((pow(Animal::AnimalList[1]->VelX, 2) + pow(Animal::AnimalList[1]->VelY, 2))) == 0 ) { 
			Animal::AnimalList[1]->VelX = 3 * ( Animal::AnimalList[0]->X - Animal::AnimalList[1]->X ) / (sqrt(pow(Animal::AnimalList[0]->X - Animal::AnimalList[1]->X, 2) + pow(Animal::AnimalList[0]->Y - Animal::AnimalList[1]->Y, 2)));
			Animal::AnimalList[1]->VelY = 3 * ( Animal::AnimalList[0]->Y - Animal::AnimalList[1]->Y ) / (sqrt(pow(Animal::AnimalList[0]->X - Animal::AnimalList[1]->X, 2) + pow(Animal::AnimalList[0]->Y - Animal::AnimalList[1]->Y, 2)));
		}
	} //above for testing */
		for(int i = 0; i < Entity::EntityList.size(); i++) { 
		if(Entity::EntityList[i] != NULL) Entity::EntityList[i]->OnLoop();
	}

	for(int i = 0; i < Animal::AnimalList.size(); i++) {
		for(int j = 0; j < Plant::PlantList.size(); j++) {
			Animal::AnimalList[i]->PlantCollides(Plant::PlantList[j]);
		}
	}

	for(int i = 0; i < EntityCol::EntityColList.size(); i++) {
		SkipCol = false;
		Entity* EntityA = EntityCol::EntityColList[i].EntityA;
		Entity* EntityB = EntityCol::EntityColList[i].EntityB;

		if(EntityA == NULL || EntityB == NULL) continue;
		if(EntityA->Type == Entity::ENTITY_TYPE_ANIMAL && EntityB->Type == Entity::ENTITY_TYPE_ANIMAL) {
			for(int j = 0; j < i; j++) {
				if( ( ( EntityA == EntityCol::EntityColList[j].EntityA && EntityB == EntityCol::EntityColList[j].EntityB ) || ( EntityA == EntityCol::EntityColList[j].EntityB && EntityB == EntityCol::EntityColList[j].EntityA ) ) && ( EntityCol::EntityColList[j].BodyCol == EntityCol::EntityColList[i].BodyCol ) ) {
					SkipCol = true;
					break;
				}
			}
		}
		if(!SkipCol) {
			if(EntityA->OnCollision(EntityB, EntityCol::EntityColList[i].BodyCol)) EntityB->OnCollision(EntityA, EntityCol::EntityColList[i].BodyCol);
		}
	}
	
	EntityCol::EntityColList.clear();

	for(int i = 0; i < Entity::EntityList.size(); i++) {
		if(Entity::EntityList[i]->Type == Entity::ENTITY_TYPE_MISC) {
			if(Entity::EntityList[i]->Dead) {
				delete Entity::EntityList[i];
				Entity::EntityList.erase(Entity::EntityList.begin() + i);
			}
		}
	}

	for(int i = 0; i < Entity::EntityList.size(); i++) {
		for(int j = 0; j < Plant::PlantList.size(); j++) {
			if((Entity::EntityList[i] == Plant::PlantList[j]) && Plant::PlantList[j]->Dead) {
				Plant::PlantList.erase(Plant::PlantList.begin() + j);
				delete Entity::EntityList[i];
				Entity::EntityList.erase(Entity::EntityList.begin() + i);
			}
		}

		if(i >= Entity::EntityList.size()) break;
	}
	
	for(int i = 0; i < Entity::EntityList.size(); i++) {
		for(int j = 0; j < Animal::AnimalList.size(); j++) {
				if((Entity::EntityList[i] == Animal::AnimalList[j]) && Animal::AnimalList[j]->Dead) {

					if(Animal::AnimalList.size() == 1) { 
						Animal* tempAnimal = NULL;
						for(int i = 0; i < 9; i++) {
							Animal* tempAnimal = new Animal( (i % 3) * Map::theMap->Width / 3, (i/3) * Map::theMap->Height / 3, 1000, 10 + rand() % 22, 3);
							Entity::EntityList.push_back(tempAnimal); 
							Animal::AnimalList.push_back(tempAnimal);
						}
					}

					Animal::AnimalList.erase(Animal::AnimalList.begin() + j);
					delete Entity::EntityList[i];
					Entity::EntityList.erase(Entity::EntityList.begin() + i);
				}
			}

		if(i >= Entity::EntityList.size()) break;
	}

	if(Plant::PlantList.size() < 1) {
		Plant* tempPlant = NULL;
		int PlantCounter = 0; //to set a minimum number of plants
		int ID = 0;
		int InitialNumPlants = 20;

		for(int i = 1 ; i <= InitialNumPlants; i++) {
			ID = rand() % (Map::theMap->TileList.size() / InitialNumPlants) + (Map::theMap->TileList.size() * (i - 1) / InitialNumPlants);
			if(ID - i*((Map::theMap->TileList.size()) / InitialNumPlants) >= 3) ID -= 3;
			PlantCounter = 0;
			for(int j = 0; j < 10; j++) {
				if(Map::theMap->TileList[ID].TypeID != Tile::TILE_TYPE_WATER && !(ID >= Map::theMap->TileList.size())) {
					tempPlant = new Plant((ID % (Map::theMap->Width/Tile::Size))*Tile::Size, (ID/(Map::theMap->Width/Tile::Size))*Tile::Size);
					if(tempPlant != NULL) {
						Entity::EntityList.push_back(tempPlant);
						Plant::PlantList.push_back(tempPlant);
						PlantCounter++;
						ID++;
					}
				}
				if(ID >= Map::theMap->TileList.size() || ID >= i*((Map::theMap->TileList.size()) / InitialNumPlants) || PlantCounter >=1) break;
			}
		}
	}
	
	/*
	if(Animal::AnimalList.size() < 2) {

		Animal* tempAnimal = NULL;
		for(int i = 0; i < 9; i++) {
			tempAnimal = new Animal(Animal::AnimalList[0], &Animal::AnimalList[0]->theBrain);
			tempAnimal->X = (i % 3) * Map::theMap->Width / 3;
			tempAnimal->Y = (i/3) * Map::theMap->Height / 3;
			tempAnimal->CurrentEnergy = tempAnimal->MaxEnergy;

			Entity::EntityList.push_back(tempAnimal); 
			Animal::AnimalList.push_back(tempAnimal);
		}
	}
	*/
}

void World::OnRender(SDL_Surface* Surf_Display) {
	SDL_FillRect(Surf_Display, NULL, 0);

	Map::theMap->OnRender(Surf_Display, -Camera::theCamera->X, -Camera::theCamera->Y);
	//render entities
	for(int i = 0; i < Plant::PlantList.size(); i++) {
		Plant::PlantList[i]->OnRender(Surf_Display);
	}

	for(int i = 0; i < Entity::EntityList.size(); i++) {
		if(Entity::EntityList[i]->Type == Entity::ENTITY_TYPE_MISC) Entity::EntityList[i]->OnRender(Surf_Display);
	}
	
	for(int i = 0; i < Animal::AnimalList.size(); i++) {
		Animal::AnimalList[i]->OnRender(Surf_Display);
	}
}

void World::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) {
		case SDLK_ESCAPE:
			AppStateManager::theAppStateManager->AppState = AppStateManager::PAUSE_MENU;
		case SDLK_r:
			if(Animal::AnimalList.size() != 0) {
				int UpperBound = Animal::AnimalList.size();
				for(int i = 0; i < UpperBound; i++) {
					Animal::AnimalList[i]->OnWorldr();
				}
			}
	}
}

void World::OnLButtonDown(int mX, int mY) {
}

void World::OnLButtonUp(int mX, int mY) {
}
