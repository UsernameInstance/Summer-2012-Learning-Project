#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "Surface.h"

class Button {
	public:
		int X;
		int Y;

		int TypeID; //to be defined by its menu

		int Width;
		int Height;
		bool Clicked;  //clicked = down

		SDL_Surface* Surf_Button;

		Button(int TypeID = 0, int X = 0, int Y = 0, int Width = 160, int Height = 120);
		~Button();

		bool OnClick();
};


#endif