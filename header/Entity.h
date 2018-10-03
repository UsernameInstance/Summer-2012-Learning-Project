#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <vector>
#include <cmath>
#include "Surface.h"
#include "Camera.h"
#include "FPS.h"
#include "Map.h"
#include "WorldSettings.h"

class Entity {
	public:
		static std::vector<Entity*> EntityList;

		static float JoulesPerFat,  JoulesPerMuscle, JouleDrainPerKilo, MFG, MR, EDPN, CoFr; //MFG = MuscleFatigueGain, MR = MuscleRecovery, EDPN = Energy Drain Per Newton; CoFr = Coefficient of Friction

		Entity(float X = 0, float Y = 0, int Type = ENTITY_TYPE_MISC, char* File = NULL); //4.2 Kilo Joules per 1000 cals = 1 Cal
		virtual ~Entity();

		Entity(Entity* SourceEntity);

		SDL_Surface* Surf_Basis; //Used to keep a back up of the image to be drawn to the screen in order to bypass image degradation due to surface manipulations.
		SDL_Surface* Surf_Entity; //the image that's actually drawn to the screen.
		SDL_Surface* Surf_Working; //Used in doing surface manipulations (like rotations, resizing, etc...)

		float X, Y;

		float VelX; //Vel for Velocity.  X velocity: + right, - left.  Y velocity: + down, - Up.
		float VelY;

		float AccelX;
		float AccelY;

		float NormalForce; //Mass * Gravity
		float DragForceX; //(1/2) * 0.4 * WorldSettings::AtmDensity * (pow(VelX, 2)*(Radius/32))
		float DragForceY; //(1/2) * 0.4 * WorldSettings::AtmDensity * (pow(VelY, 2)*(Radius/32))
		float NetForceX; //Net Force on Entity.  In Newtons.
		float NetForceY;
		float ViscousTorque;

		int Type;
		int CreationTime;  //Time of creation after program start.  In milliseconds;

		bool Dead;
		bool OffMap; //are the entity coordinates off the map?

		virtual bool PosValid(float NewX, float NewY);
		virtual bool PosValidMap(int NewX, int NewY);
		virtual bool PosValidEntity(Entity* TargetEntity, int NewX, int NewY);

		virtual void OnLoop();
		virtual void OnRender(SDL_Surface* Surf_Display);
		virtual bool OnCollision(Entity* TargetEntity, bool BodyCol = false);

		enum {
			ENTITY_TYPE_ANIMAL,
			ENTITY_TYPE_PLANT,
			ENTITY_TYPE_MISC
		};
};


class EntityCol {
	public:
		static std::vector<EntityCol> EntityColList;
		bool BodyCol;
		Entity* EntityA;
		Entity* EntityB;

		EntityCol();
};
#endif