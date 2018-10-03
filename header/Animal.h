#ifndef ANIMAL_H
#define ANIMAL_H

#include "Entity.h"
#include "Organ.h"
#include "Plant.h"
#include "Brain.h"

class Animal: public Entity {
	public:
		static std::vector<Animal*> AnimalList;
		Animal(float X = 0, float Y = 0, float InputEnergy = 1000, int HiddenLayerSize = 10 + rand() % 20, int OutputLayerSize = 3);  //Makes random
		~Animal();

		Animal(Animal* SourceAnimal, Brain* SourceBrain);

		Animal(Animal* SourceAnimal, Entity* SourceEntity, Brain* SourceBrain);

		std::vector<Organ*> OrganList;

		Brain theBrain;

		SDL_Surface* Surf_Body; //used to keep a copy of the circular body surface.  This is to allow dynamic resizing.
		float Angle; //0 => (0, -1) pointing, in game basis
		
		float LifeSpan; //LifeSpan and Age will be in minutes
		float Age;
	
		float CurrentEnergy;
		float MaxEnergy;

		float MaxTotalMass, MaxFatMass, MaxMuscleMass;  //In Kilograms
		float CurrentTotalMass, CurrentFatMass, CurrentMuscleMass;
		float RatioFatBurn; //Fat used per Joule / Muscle Used per Joule

		float MaxForceOutput; //possible
		float NewVelX, NewVelY;
		float Torque; //positive => counter-clockwise
		float AngularAccel;
		float AngularVel;
		float NewAngularVel;
		float MaxTorqueOutput;

		float Radius; //will have circular body 32 pixels = 1 meter at 1xZoomfactor.  Radius/32 for calculations.

		float LastColTime;

		float LastReproductionTime; //used to limit reproduction attempts

		float FM; //FatigueMultiplier

		struct RGBColor8 {
			Uint8 R;
			Uint8 G;
			Uint8 B;
			//Uint32 HexColor;  //Hexcolor = (R << 16) + (G << 8) + B
		};

		RGBColor8 Color;

		void OnLoop();
		void OnRender(SDL_Surface* Surf_Display);

		void OnThink();
		void OnBrainOutput();

		void OnMove(float MoveX, float MoveY);
		void OnRotate(float AngularMove);

		void OnForceOutput(float FX, float FY);
		void OnTorqueOutput(float T);
		bool AnimalCollides(Animal* TargetAnimal, float NewX, float NewY);
		bool PlantCollides(Plant* TargetPlant);
		bool OnCollision(Entity* TargetEntity, bool BodyCol = false);
		bool PosValid(float NewX, float NewY);
		void OnCollisionAnimal(Animal* TargetAnimal, bool BodyCol = false);
		bool OnReproduce(Animal* TargetAnimal = NULL, float PercentInputEnergy = 0.5);
		bool OnWorldr();
		Animal* GetAnimal(float X, float Y);
		/*
		
		bool PosValidEntity(Entity* TargetEntity, int NewX, int NewY);
		
		*/
};

class BloodSplatter: public Entity {
	public:
		BloodSplatter(float X, float Y);

		void OnRender(SDL_Surface* Surf_Display);
		void OnLoop();
};

class Corpse: public Entity {
	public:
		Corpse(float X, float Y);

		void OnRender(SDL_Surface* Surf_Display);
		void OnLoop();
};

class ReproduceAnimation: public Entity {
	public:
		ReproduceAnimation(float X, float Y);

		void OnRender(SDL_Surface* Surf_Display);
		void OnLoop();
};
#endif