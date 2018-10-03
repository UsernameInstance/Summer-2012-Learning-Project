#include "WorldSettings.h"


namespace WorldSettings {
		int Width; //will be spherical eventually.  For now, square.  Will use default camera scale to be 32 pixels per meter.  Zoom in max -> 128 pixels/meter.  Zoom out max -> 8 pixels per meter.
		int Height;

		float Gravity;  //affects friction force from tiles.  F = u*m*Gravity
		float AtmDensity; //Short for Atmosphere Density.  Used in Drag force equations.  probably use Fdrag = (1/2)*Drag*AtmDensity(velocity^2)*(object size)

		float MutationRate;  //Probability of Mutation per Data Field copy
		int NumAnimals;
		int NumPlants;
		int MaxNumAnimals;
		int MaxNumPlants;

		int WorldSpeed;  //used to set simSpeed.
}
