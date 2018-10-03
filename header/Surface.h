#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

class Surface {
	public:
		Surface();

		//For loading file into memory.
		static SDL_Surface* OnLoad(char* File);  

		//For rendering whole images.
		static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
		
		//For rendering part of images
		static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
		
		//Removes designated RGB value from image.
		static bool Transparent(SDL_Surface* Surf_Dest, int R, int G, int B);

		static Uint32 GetPixel32(SDL_Surface* Surface, int X, int Y);

		static void SetPixel32(SDL_Surface* Surface, int X, int Y, Uint32 Color);

		static void SetSurfColor(SDL_Surface* Surface, Uint32 OriginalColor, Uint32 NewColor);

		static SDL_Surface* ScaleSurface(SDL_Surface* Surface, Uint32 Width, Uint32 Height);

		static SDL_Surface* RotateSurface(SDL_Surface* Surface, double Angle); //Angle in radians.  Rotation counter-clockwise.
};


#endif