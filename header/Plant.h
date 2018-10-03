#ifndef PLANT_H
#define PLANT_H

#include "Entity.h"
#include "Organ.h"

class Plant: public Entity {
	public:
		static std::vector<SDL_Surface*> PlantFrameList;
		static std::vector<Plant*> PlantList;

		Plant(float X, float Y);
		~Plant();

		float Lifespan; //LifeSpan and Age will be in minutes
		float Age;
		float MaxEnergy;
		float CurrentEnergy;
		float OldEnergy; //used for age calculations;
		float Radius;
		float LReAtTime;

	
		struct RGBColor8 {
			Uint8 R;
			Uint8 G;
			Uint8 B;
				//Uint32 HexColor;  //Hexcolor = (R << 16) + (G << 8) + B
		};

		static RGBColor8 Color;

		static void OnInit();
		void OnLoop();
		void OnRender(SDL_Surface* Surf_Display);
		bool OnCollision(Entity* TargetEntity, bool BodyCol = false);

		bool OnReproduce(int ID);
};

#endif 