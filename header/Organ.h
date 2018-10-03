#ifndef ORGAN_H
#define ORGAN_H

#include <SDL.h>
#include <vector>
#include <ctime>
#include <cmath>
#include "Surface.h"
#include "WorldSettings.h"

class Organ {
public:
	Organ(int Type, float AnimalRadius, float Angle = 0, float X = 0, float Y = 0); //random make
	//Organ(int Type, int AnimalRadius, float Attribute1, float Attribute2, float X, float Y);
	~Organ();

	Organ(Organ* SourceOrgan, float X, float Y, float NewAnimalRadius = 0.0, bool Mutate = false);

	SDL_Surface* Surf_Organ;

	bool Invisible;

	float X;  //position relative to creature
	float Y;
	float Angle; //Rotation Angle of Surf_Organ
	float AnimalRadius;

	int Type;

	int Width, Height; //Determined by Attribute values.

	float Attribute1; //Brain: Speed. Spike: Attack Power. Eye: Width.
	float Attribute2; //Brain: Memory. Spike: Defense Power. Eye: Depth.

	void SetDimensions(float AnimalRadius);
	void SetColor();

	void OnRender(SDL_Surface* Surf_Display);

	struct RGBColor8 {
		Uint8 R;
		Uint8 G;
		Uint8 B;
	};

	RGBColor8 Color;

	enum {
		ORGAN_TYPE_BRAIN,
		ORGAN_TYPE_SPIKE,
		ORGAN_TYPE_EYE,
		/*ORGAN_TYPE_EAR,
		ORGAN_TYPE_NOSE,
		ORGAN_TYPE_FEELER,
		*/  //will add later
	};
};


#endif