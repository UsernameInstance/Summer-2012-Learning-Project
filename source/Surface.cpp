#include "Surface.h"

Surface::Surface() {
}

//using SDL_image version of loading to support multiple image file formats.
SDL_Surface* Surface::OnLoad(char* File) {
	SDL_Surface* Surf_Temp = NULL;
	SDL_Surface* Surf_Return = NULL;

	Surf_Temp = IMG_Load(File);  //SDL_image load function

	if (Surf_Temp == NULL) return NULL;

	Surf_Return = SDL_DisplayFormat(Surf_Temp);

	SDL_FreeSurface(Surf_Temp);

	return Surf_Return;
}

//Surf_Dest is the surface we draw to/on.  Surf_Src is what we are drawing/rendering.  X is the X coordinate of where it will be drawn.  Y is the Y coordinate of where it will be drawn.
// Top leftmost pixel of the display is (0,0) i.e. X = 0, Y = 0.  Down is +Y right is +X.
bool Surface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {

	if (Surf_Dest == NULL || Surf_Src == NULL) return false;

	//Creates rectange with four parameters, x-coordinate = x, y-coordinate = y, width = w, height = h.
	//I'm assuming x and y refer to the top left of the rectangle, so that (0,0) will position it to top left of display.
	SDL_Rect DestR;
	
	DestR.x = X;
	DestR.y = Y;

	//Blit surface means "draw".  Blitting is "drawing" or "rendering.
	//The NULL parameter is for SDL_Rect use.
	SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

	return true;
}

//For rendering parts of images rather than whole images.
bool Surface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
	if (Surf_Dest == NULL || Surf_Src == NULL) 
	{
		return false;
	}

	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	// Following designates what to draw from src file. (X2,Y2) will denote upper left of rectangular portion to be drawn.
	// W and H denote the width and height of said rectangle.  Of course top leftmost pixel is (0,0), with right +x direction and down +y direction.
	// Width is calculate from X2 to the right, and Height from Y2 down.  So vertices of rectangle would be (X2,Y2), (X2 + W, Y2), (X2, Y2 + H), (X2 + W, Y2 + H)
	SDL_Rect SrcR;
	SrcR.x = X2;
	SrcR.y = Y2;
	SrcR.w = W;
	SrcR.h = H;

	SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);

	return true;
}

bool Surface::Transparent(SDL_Surface* Surf_Dest, int R, int G, int B) {
	if(Surf_Dest == NULL) return false;

	SDL_SetColorKey(Surf_Dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Dest->format, R, G, B));

	return true;
}


Uint32 Surface::GetPixel32(SDL_Surface* Surface, int X, int Y) {
	if(SDL_MUSTLOCK(Surface)) SDL_LockSurface(Surface);
	Uint32* Pixels = (Uint32 *)Surface->pixels;  //Convert pixel to Uint32 Pointer
	if(SDL_MUSTLOCK(Surface)) SDL_UnlockSurface(Surface);

	return Pixels[Y * (Surface->pitch / 4) + X];  //Returns Uint32 color of pixel at x, y;
}

void Surface::SetPixel32(SDL_Surface* Surface, int X, int Y, Uint32 Color) {
	//if(SDL_MUSTLOCK(Surface)) SDL_LockSurface(Surface);
	Uint32* Pixels = (Uint32 *)Surface->pixels; //Converts pixels to Uint32

	Pixels[ Y * (Surface->pitch / 4) + X ] = Color;  //Sets pixel color
	//if(SDL_MUSTLOCK(Surface)) SDL_UnlockSurface(Surface);
}

void Surface::SetSurfColor(SDL_Surface* Surface, Uint32 OriginalColor, Uint32 NewColor) {  //MakeSure you obtain Surf and SrcColor using SDL_MapRGB(Surface->Format, etc..) so that the pixel data is stored correctly.
	for(int Y = 0; Y < Surface->h; Y++) {
		for(int X = 0; X < Surface->pitch / 4; X++) {
			if(GetPixel32(Surface, X, Y) == OriginalColor) SetPixel32(Surface, X, Y, NewColor);
		}
	}
}


SDL_Surface* Surface::ScaleSurface(SDL_Surface* Surface, Uint32 Width, Uint32 Height)
{
    if(!Surface || !Width || !Height)
        return 0;

    SDL_Surface* _ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
 
    double  _stretch_factor_x = (double)(Width)  / (double)(Surface->w);
	double  _stretch_factor_y = (double)(Height) / (double)(Surface->h);
 
    for(Uint32 y = 0; y < Surface->h; y++) {
        for(Uint32 x = 0; x < Surface->w; x++) {
            for(Uint32 o_y = 0; o_y < _stretch_factor_y; ++o_y) {
                for(Uint32 o_x = 0; o_x < _stretch_factor_x; ++o_x) {
                    SetPixel32(_ret, (Uint32)(_stretch_factor_x * x) + o_x, 
                        (Uint32)(_stretch_factor_y * y) + o_y, GetPixel32(Surface, x, y));
				}
			}
		}
	}

    return _ret;
}

//Core functionality working.  Rotated image isn't the BEST copy depending on the angle.  QQ
SDL_Surface* Surface::RotateSurface(SDL_Surface* Surface, double Angle) {
	if(!Surface) return 0;

	if(SDL_MUSTLOCK(Surface)) SDL_LockSurface(Surface);

	SDL_Surface* _ret = SDL_CreateRGBSurface(Surface->flags, Surface->w, Surface->h, Surface->format->BitsPerPixel, Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

	double CX = Surface->w / 2, CY = Surface->h / 2; //Center coordinates of image, or close enough.
	double X, Y, X2, Y2;

	for(Uint32 y = 0; y < Surface->h; y++) {
        for(Uint32 x = 0; x < Surface->w; x++) {
			X = (double)x - CX;
			Y = (double)y - CY;
			X2 = (X * cos(Angle) - Y * sin(Angle));
			Y2 = (X * sin(Angle) + Y * cos(Angle));
			X2 += CX;
			Y2 += CY;
			if( (int)X2 >= (int)Surface->w || (int)X2 < 0 || (int)Y2 >= Surface->h || (int)Y2 < 0) SetPixel32(_ret, x, y, SDL_MapRGB(Surface->format, 255, 0, 255));
			else SetPixel32(_ret, x, y, GetPixel32(Surface, X2, Y2));
		}
	}

	if(SDL_MUSTLOCK(Surface)) SDL_UnlockSurface(Surface);

	return _ret;
}