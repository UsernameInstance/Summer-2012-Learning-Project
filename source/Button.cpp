#include "Button.h"

Button::Button(int TypeID, int X, int Y, int Width, int Height) {
	this->TypeID = TypeID;
	this->X = X;
	this->Y = Y;
	this->Width = Width;
	this->Height = Height;

	Clicked = false;
}

Button::~Button() {
	if(Surf_Button != NULL) SDL_FreeSurface(Surf_Button);
}

bool Button::OnClick() {
	if(!Clicked) Clicked = true;
	else if(Clicked) Clicked = false;
	return Clicked;
}
