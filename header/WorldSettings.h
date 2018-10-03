#ifndef WORLDSETTING_H
#define WORLDSETTINGS_H

namespace WorldSettings {
		extern int Width; //will be spherical eventually.  For now, square.  Will use default camera scale to be 32 pixels per meter.  Zoom in max -> 128 pixels/meter.  Zoom out max -> 8 pixels per meter.
		extern int Height;

		extern float Gravity;  //affects friction force from tiles.  F = u*m*Gravity
		extern float AtmDensity; //Short for Atmosphere Density.  Used in Drag force equations.  probably use Fdrag = (1/2)*Drag*AtmDensity(velocity^2)*(object size)

		extern float MutationRate;  //Probability of Mutation per Data Field copy
		extern int NumAnimals;
		extern int NumPlants;
		extern int MaxNumAnimals;
		extern int MaxNumPlants;

		extern int WorldSpeed;  //used to set simSpeed.
}

#endif