#include "Organ.h"

Organ::Organ(int Type, float AnimalRadius, float Angle, float X, float Y) {
	this->Type = Type;
	Invisible = false;

	this->AnimalRadius = AnimalRadius;

	float CXAnimal = X + (AnimalRadius); //center coordinates
	float CYAnimal = Y + (AnimalRadius);
	float CX = (float)0/2;
	float CY = (float)0/2;
	float CX2, CY2; //for transform

	switch(Type) {
		case ORGAN_TYPE_BRAIN:
			Width = Height = 0;
			Attribute1 = float(30 + rand() % 71)/float(100) * float(5); //speed
			Attribute2 = float(30 + rand() % 71)/float(100) * float(2); //memory
			Surf_Organ = NULL;
			Invisible = true;
			break;

		case ORGAN_TYPE_SPIKE:
			Attribute1 = float(30 + rand() % 71)/float(100) * float(3);  
			Attribute2 = float(30 + rand() % 71)/float(100) * float(3);
			Surf_Organ = Surface::OnLoad("../Debug/gfx/OrganSpikeTest.bmp");
			CX = X + AnimalRadius;  //Place center near perimeter of Animal.
			CY = Y + AnimalRadius/float(20);
			break;

		case ORGAN_TYPE_EYE:
			Attribute1 = (0.5698 + (float(rand() % 8605)/10000.0)) * 2.15;  //1 meter = 32 pixels at default zoom, width
			Attribute2 = (0.5698 + (float(rand() % 8605)/10000.0)) * 2.15;  //depth
			Surf_Organ = Surface::OnLoad("../Debug/gfx/OrganEyeTest.bmp");
			CX = X + AnimalRadius;  //Place center near perimeter of Animal.
			CY = Y + AnimalRadius;
			break;
	}

	Color.R = 75 + rand() % 182;
	Color.G = 75 + rand() % 182;
	Color.B = 75 + rand() % 182;
	
	this->Angle = Angle;
	
	CX2 = ((CX - CXAnimal) * cos(Angle) + (CY - CYAnimal) * sin(Angle));
	CY2 = (-(CX - CXAnimal) * sin(Angle) + (CY - CYAnimal) * cos(Angle));
	CX = CX2 + CXAnimal;
	CY = CY2 + CYAnimal;
	
	SDL_Surface* tempSurf = Surface::RotateSurface(Surf_Organ, Angle);
	SDL_FreeSurface(Surf_Organ);
	Surf_Organ = tempSurf;
	SetDimensions(AnimalRadius);
	SetColor();

	this->X = CX - Width/float(2);
	this->Y = CY - Height/float(2);
}

Organ::Organ(Organ* SourceOrgan, float X, float Y, float NewAnimalRadius, bool Mutate) {
	this->Type = SourceOrgan->Type;
	Invisible = false;
	
	float SSize = 1.0/WorldSettings::MutationRate;
	float MutationM;
	MutationM = float(75 + rand() % 50)/100.0;

	if(Mutate && NewAnimalRadius == 0) NewAnimalRadius = SourceOrgan->AnimalRadius;

	if(Mutate) {
		float CXAnimal = X + (NewAnimalRadius); //center coordinates
		float CYAnimal = Y + (NewAnimalRadius);
		float CX = (float)Width/float(2);
		float CY = (float)Height/float(2);
		float CX2, CY2; //for transform

		switch(Type) {
			case ORGAN_TYPE_BRAIN:
				Width = Height = 0;
				
				MutationM = float(75 + rand() % 50)/100.0;
				if( !(rand() % int(SSize)) ) { 
					if( SourceOrgan->Attribute1 * MutationM < 0.75 ) Attribute1 = 0.75;
					else if ( SourceOrgan->Attribute1 * MutationM > 10 ) Attribute1 = 10;
					else Attribute1 = SourceOrgan->Attribute1 * MutationM; 
				}
				else Attribute1 = SourceOrgan->Attribute1;//speed

				MutationM = float(75 + rand() % 50)/100.0;
				if( !(rand() % int(SSize)) ) { 
					if( SourceOrgan->Attribute2 * MutationM < 0.3 ) Attribute2 = 0.3;
					else if ( SourceOrgan->Attribute2 * MutationM > 4 ) Attribute2 = 4;
					else Attribute2 = SourceOrgan->Attribute2 * MutationM; 
				}
				else Attribute2 = SourceOrgan->Attribute2;//memory
				
				Surf_Organ = NULL;
				Invisible = true;
				break;

			case ORGAN_TYPE_SPIKE:
				MutationM = float(75 + rand() % 50)/100.0;
				if( !(rand() % int(SSize)) ) { 
					if( SourceOrgan->Attribute1 * MutationM < 0.45 ) Attribute1 = 0.45;
					else if ( SourceOrgan->Attribute1 * MutationM > 6 ) Attribute1 = 6;
					else Attribute1 = SourceOrgan->Attribute1 * MutationM; 
				}
				else Attribute1 = SourceOrgan->Attribute1;

				MutationM = float(75 + rand() % 50)/100.0;
				if( !(rand() % int(SSize)) ) { 
					if( SourceOrgan->Attribute2 * MutationM < 0.45 ) Attribute2 = 0.45;
					else if ( SourceOrgan->Attribute2 * MutationM > 6 ) Attribute2 = 6;
					else Attribute2 = SourceOrgan->Attribute2 * MutationM; 
				}
				else Attribute2 = SourceOrgan->Attribute2;

				Surf_Organ = Surface::OnLoad("../Debug/gfx/OrganSpikeTest.bmp");
				CX = X + NewAnimalRadius;  //Place center near perimeter of Animal.
				CY = Y + NewAnimalRadius/float(20);
				break;

			case ORGAN_TYPE_EYE:
				MutationM = float(75 + rand() % 50)/100.0;
				if( !(rand() % int(SSize)) ) { 
					if( SourceOrgan->Attribute1 * MutationM < 0.3 ) Attribute1 = 0.3;
					else if ( SourceOrgan->Attribute1 * MutationM > 4 ) Attribute1 = 4;
					else Attribute1 = SourceOrgan->Attribute1 * MutationM; 
				}
				else Attribute1 = SourceOrgan->Attribute1;

				MutationM = float(75 + rand() % 50)/100.0;
				if( !(rand() % int(SSize)) ) { 
					if( SourceOrgan->Attribute2 * MutationM < 0.3 ) Attribute2 = 0.3;
					else if ( SourceOrgan->Attribute2 * MutationM > 4 ) Attribute2 = 4;
					else Attribute2 = SourceOrgan->Attribute2 * MutationM; 
				}
				else Attribute2 = SourceOrgan->Attribute2;

				Surf_Organ = Surface::OnLoad("../Debug/gfx/OrganEyeTest.bmp");
				CX = X + NewAnimalRadius;  //Place center near perimeter of Animal.
				CY = Y + NewAnimalRadius;
				break;
		}

		MutationM = float(75 + rand() % 50)/100.0;

		if( !(rand() % (int)SSize ) ) { 
			if(int((float)SourceOrgan->Color.R * MutationM) > 255) Color.R = 255; 
			else if(int((float)SourceOrgan->Color.R * MutationM) < 0) Color.R = 0; 
			else Color.R = Uint8((float)SourceOrgan->Color.R * MutationM); 
			
		}
		else Color.R = SourceOrgan->Color.R;

		MutationM = float(75 + rand() % 50)/100.0;

		if( !(rand() % (int)SSize ) ) { 
			if(int((float)SourceOrgan->Color.G * MutationM) > 255) Color.G = 255; 
			else if(int((float)SourceOrgan->Color.G * MutationM) < 0) Color.G = 0; 
			else Color.G = Uint8((float)SourceOrgan->Color.G * MutationM); 
		}
		else Color.G = SourceOrgan->Color.G;

		MutationM = float(75 + rand() % 50)/100.0;

		if( !(rand() % (int)SSize ) ) { 
			if(int((float)SourceOrgan->Color.B * MutationM) > 255) Color.B = 255; 
			else if(int((float)SourceOrgan->Color.B * MutationM) < 0) Color.B = 0; 
			else Color.B = Uint8((float)SourceOrgan->Color.B * MutationM); 
		}
		else Color.B = SourceOrgan->Color.B;
		
		this->Angle = 0;
		//this->Angle = SourceOrgan->Angle;
	
		CX2 = ((CX - CXAnimal) * cos(Angle) + (CY - CYAnimal) * sin(Angle));
		CY2 = (-(CX - CXAnimal) * sin(Angle) + (CY - CYAnimal) * cos(Angle));
		CX = CX2 + CXAnimal;
		CY = CY2 + CYAnimal;
	
		AnimalRadius = NewAnimalRadius;

		SDL_Surface* tempSurf = Surface::RotateSurface(Surf_Organ, Angle);
		SDL_FreeSurface(Surf_Organ);
		Surf_Organ = tempSurf;
		SetDimensions(AnimalRadius);
		SetColor();

		this->X = CX - float(Width)/float(2);
		this->Y = CY - float(Height)/float(2);

	}
	else {
		float CXAnimal = X + (AnimalRadius); //center coordinates
		float CYAnimal = Y + (AnimalRadius);
		float CX = (float)Width/float(2);
		float CY = (float)Height/float(2);
		float CX2, CY2; //for transform

		switch(Type) {
			case ORGAN_TYPE_BRAIN:
				Width = Height = 0;
				Attribute1 = SourceOrgan->Attribute1; //speed
				Attribute2 = SourceOrgan->Attribute2; //memory
				Surf_Organ = NULL;
				Invisible = true;
				break;

			case ORGAN_TYPE_SPIKE:
				Attribute1 = SourceOrgan->Attribute1;
				Attribute2 = SourceOrgan->Attribute2;
				Surf_Organ = Surface::OnLoad("../Debug/gfx/OrganSpikeTest.bmp");
				CX = X + AnimalRadius;  //Place center near perimeter of Animal.
				CY = Y + AnimalRadius/float(20);
				break;

			case ORGAN_TYPE_EYE:
				Attribute1 = SourceOrgan->Attribute1; //1 meter = 32 pixels at default zoom, width
				Attribute2 = SourceOrgan->Attribute2; //depth
				Surf_Organ = Surface::OnLoad("../Debug/gfx/OrganEyeTest.bmp");
				CX = X + AnimalRadius;  //Place center near perimeter of Animal.
				CY = Y + AnimalRadius;
				break;
		}

		Color.R = SourceOrgan->Color.R, Color.G = SourceOrgan->Color.G, Color.B = SourceOrgan->Color.B;
		
		this->Angle = 0;
		//this->Angle = SourceOrgan->Angle;
	
		CX2 = ((CX - CXAnimal) * cos(Angle) + (CY - CYAnimal) * sin(Angle));
		CY2 = (-(CX - CXAnimal) * sin(Angle) + (CY - CYAnimal) * cos(Angle));
		CX = CX2 + CXAnimal;
		CY = CY2 + CYAnimal;
	
		AnimalRadius = SourceOrgan->AnimalRadius;

		SDL_Surface* tempSurf = Surface::RotateSurface(Surf_Organ, Angle);
		SDL_FreeSurface(Surf_Organ);
		Surf_Organ = tempSurf;
		SetDimensions(AnimalRadius);
		SetColor();

		this->X = CX - float(Width)/float(2);
		this->Y = CY - float(Height)/float(2);
	}
}
/*
Organ::Organ(int Type, int AnimalRadius, float Attribute1, float Attribute2, float X, float Y) {
	this->Type = Type;

	if(Type != ORGAN_TYPE_BRAIN) {
		this->Width = Width;
		this->Height = Height;
	}
	else {
		this->Width = 0;
		this->Height = 0;
	}

	this->Attribute1 = Attribute1;
	this->Attribute2 = Attribute2;
	this->X = X;
	this->Y = Y;

}*/

Organ::~Organ() { 
	if(Surf_Organ != NULL) SDL_FreeSurface(Surf_Organ);
}

void Organ::OnRender(SDL_Surface* Surf_Display) {
	if(!Invisible)Surface::OnDraw(Surf_Display, Surf_Organ, (int) X, (int) Y, 0, 0, Surf_Organ->w, Surf_Organ->h);
}

void Organ::SetDimensions(float AnimalRadius) {
	switch(Type) {
		case ORGAN_TYPE_SPIKE:
			Width = (int) ((Attribute1/(Attribute1 + Attribute2)) * AnimalRadius * (float(2)));
			Height = (int) ((Attribute2/(Attribute1 + Attribute2)) * AnimalRadius * (float(2)));
			break;

		case ORGAN_TYPE_EYE:
			Width = (int) ((Attribute1/(Attribute1 + Attribute2)) * AnimalRadius * float(2));
			Height = (int) ((Attribute2/(Attribute1 + Attribute2)) * AnimalRadius * float(2));
			break;
	}

	SDL_Surface* tempSurf = Surf_Organ;
		Surf_Organ = Surface::ScaleSurface(tempSurf, Width, Height);// SurfWidthScale, SurfHeightScale);
		SDL_FreeSurface(tempSurf); //ScaleSurface made a copy.
}

void Organ::SetColor() {
	if(Type && Width >= 1 && Height >= 1) {
		Surface::SetSurfColor(Surf_Organ, SDL_MapRGB(Surf_Organ->format, 100, 100, 100), SDL_MapRGB(Surf_Organ->format, Color.R, Color.G , Color.B));
		Surface::Transparent(Surf_Organ, 255, 0, 255);
	}
	else if(Type) {
		Invisible = true;
	}
}