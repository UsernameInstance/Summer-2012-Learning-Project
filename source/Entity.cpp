#include "Entity.h"

std::vector<Entity*> Entity::EntityList;

float Entity::JoulesPerFat = 37.62;  //KILO Joules per KILO gram
float Entity::JoulesPerMuscle = 16.73;
float Entity::JouleDrainPerKilo = 0.0936; //Kilo Joules per kilo gram per second drain. 
float Entity::MFG = 1.0/32.0;
float Entity::MR = 1.0/1.60;
float Entity::EDPN = .0001; //Kilo-Joules per Newton
float Entity::CoFr = 0.1;

Entity::Entity(float X, float Y, int Type, char* File) {
	this->X = X;
	this->Y = Y;
	this->Type = Type;
	CreationTime = SDL_GetTicks();
	Dead = false;
	OffMap = false;
	NetForceX  = NetForceY = 0;
	NormalForce = 0;
	DragForceX = DragForceY = 0;
	VelX = VelY = 0;
	AccelX = AccelY = 0;
	ViscousTorque = 0;
	if(File != NULL) Surf_Entity = Surface::OnLoad(File);
	else Surf_Entity = NULL;
	Surf_Working = NULL;
	Surf_Basis = NULL;
}

Entity::~Entity() {
	if(Surf_Entity != NULL) SDL_FreeSurface(Surf_Entity);
	//if(Surf_Working != NULL) SDL_FreeSurface(Surf_Working);
		
}

Entity::Entity(Entity* SourceEntity) {
	this->X = SourceEntity->X;
	this->Y = SourceEntity->Y;
	this->Type = SourceEntity->Type;
	
	CreationTime = SDL_GetTicks();
	Dead = false;
	OffMap = false;
	NetForceX  = NetForceY = 0;
	NormalForce = 0;
	DragForceX = DragForceY = 0;
	VelX = VelY = 0;
	AccelX = AccelY = 0;
	ViscousTorque = 0;

	if(SourceEntity->Surf_Entity != NULL) Surf_Entity = Surface::ScaleSurface(SourceEntity->Surf_Entity, SourceEntity->Surf_Entity->w, SourceEntity->Surf_Entity->h);
	else Surf_Entity = NULL;
}

bool Entity::PosValid(float NewX, float NewY) {

	return true; //pure virtual
}

bool Entity::PosValidMap(int NewX, int NewY){
	if(NewX < 0 || NewX > Map::theMap->Width || NewY < 0 || NewY > Map::theMap->Height) return false;
	return true;
}

bool Entity::PosValidEntity(Entity* TargetEntity, int NewX, int NewY) {
	return false; //pure virtual
}

void Entity::OnLoop() {
	//Pure Virtual
}

void Entity::OnRender(SDL_Surface* Surf_Display) {
	//Pure Virtual
}

bool Entity::OnCollision(Entity* TargetEntity, bool BodyCol) {
	//pure virtual
	return false;
}

std::vector<EntityCol> EntityCol::EntityColList;

EntityCol::EntityCol() {
	this->EntityA = NULL;
	this->EntityB = NULL;
	BodyCol = false;
}

