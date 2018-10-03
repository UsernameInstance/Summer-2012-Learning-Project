#include "Animal.h"

std::vector<Animal*> Animal::AnimalList;

Animal::Animal(float X, float Y, float InputEnergy, int HiddenLayerSize, int OutputLayerSize): Entity(X, Y, Entity::ENTITY_TYPE_ANIMAL, "../Debug/gfx/animaltest.bmp"), theBrain(27, HiddenLayerSize, OutputLayerSize) {
	//temporary until I write a random number generator
	LastColTime = 0;
	LifeSpan = (float) 1 + rand() % 10;
	Age = 0;
	LastReproductionTime = 0; 
	FM = 1;
	/*
	int i = 1;
	int ID = 1;
	while(InputEnergy > 100) {
		if(i == 3) i = 1;
		if(rand() % 2) {
			tempOrgan = new Organ(i, X, Y);
			OrganList.push_back(tempOrgan);
			delete tempOrgan;

			if(OrganList.size() == ID + 1) InputEnergy -= (OrganList[ID]->Attribute1 + OrganList[ID]->Attribute2);
			ID++;
		}
		i++;
		if(!(rand() % 4)) break;
	} 
	*/
	MaxMuscleMass = (float)35 + rand() % 6;
	MaxFatMass = MaxMuscleMass * (float(10 + rand() % 16) / 100.0);
	MaxTotalMass = MaxMuscleMass + MaxFatMass;
	RatioFatBurn = (float)0.07;

	MaxEnergy = (float)(37.62 * MaxFatMass + 16.73 * MaxMuscleMass);  //expected about 1091.43 Kilo-Joules, 4.184 kJ = 1 kCal, 9kCal = 1 kg Fat, 4kCal = 1 kg Protein

	theBrain.AnimalMaxEnergy = MaxEnergy;

	if (InputEnergy > MaxEnergy) InputEnergy = MaxEnergy;

	CurrentEnergy = InputEnergy;
	/* add later
	CurrentTotalMass = MaxTotalMass * (CurrentEnergy / MaxEnergy);
	CurrentMuscleMass = CurrentTotalMass * (MaxMuscleMass/MaxTotalMass);
	CurrentFatMass = CurrentTotalMass * (MaxFatMass/MaxTotalMass);

	Radius = (float)((MaxTotalMass * (1.7/62)) * (CurrentTotalMass/(2*MaxTotalMass) + (0.5)) * 32)/2; //pixels
	MaxForceOutput = CurrentMuscleMass * 3;
	*/

	CurrentTotalMass = MaxTotalMass;
	CurrentMuscleMass = MaxMuscleMass;
	CurrentFatMass = MaxFatMass;

	Radius = (float)((MaxTotalMass * (1.7/62.0)) * (CurrentTotalMass/(2.0*MaxTotalMass) + (0.5)) * 32.0)/2.0; //pixels
	MaxForceOutput = CurrentMuscleMass * 2.5;

	MaxTorqueOutput = MaxForceOutput*Radius/4.0;

	NormalForce = CurrentTotalMass * WorldSettings::Gravity;
	NewVelX = NewVelY = 0;
	DragForceX = DragForceY = 0;
	Torque = AngularAccel = AngularVel = NewAngularVel = Angle = 0;

	int MaxOrganSize = 0;

	Organ* tempOrgan;
	
	for(int i = 0; i < 3; i++) {
		tempOrgan = new Organ(i, Radius, 0, X, Y);
		
		OrganList.push_back(tempOrgan);
		InputEnergy -= (OrganList[i]->Attribute1 + OrganList[i]->Attribute2)*2.0;
		if(OrganList[i]->Surf_Organ != NULL) {
			if(MaxOrganSize < OrganList[i]->Surf_Organ->w) MaxOrganSize = OrganList[i]->Surf_Organ->w;
			if(MaxOrganSize < OrganList[i]->Surf_Organ->h) MaxOrganSize = OrganList[i]->Surf_Organ->h;
		}
	}
	
	
	Color.R = 75 + rand() % 182;
	Color.G = 75 + rand() % 182;
	Color.B = 75 + rand() % 182;

	int SurfWidthScale = (int)((MaxTotalMass * (1.7/62.0)) * (MaxTotalMass/(2.0*MaxTotalMass) + (0.5)) * float(Surf_Entity->w));
	int SurfHeightScale = (int)((MaxTotalMass* (1.7/62.0)) * (MaxTotalMass/(2.0*MaxTotalMass) + (0.5)) * float(Surf_Entity->h));

	Surf_Body = Surf_Entity;
	Surf_Working = Surface::ScaleSurface(Surf_Entity, SurfWidthScale, SurfHeightScale);// SurfWidthScale, SurfHeightScale);
	
	Surface::SetSurfColor(Surf_Working, SDL_MapRGB(Surf_Working->format, 0, 0, 0) , SDL_MapRGB(Surf_Working->format, Color.R, Color.G , Color.B));
	Surface::Transparent(Surf_Working, 255, 0, 255);

	// Buggy sometimes crashes :(
	Surf_Basis = SDL_CreateRGBSurface(Surf_Entity->flags, Surf_Entity->w + 2*MaxOrganSize, Surf_Entity->h + 2*MaxOrganSize, Surf_Entity->format->BitsPerPixel, Surf_Entity->format->Rmask, Surf_Entity->format->Gmask, Surf_Entity->format->Bmask, Surf_Entity->format->Amask);
	Surface::SetSurfColor(Surf_Basis, SDL_MapRGB(Surf_Basis->format, 0, 0, 0), SDL_MapRGB(Surf_Basis->format, 255, 0, 255));
	Surface::Transparent(Surf_Basis, 255, 0, 255);
	Surface::OnDraw(Surf_Basis, Surf_Working, Surf_Basis->w / 2 - Surf_Working->w / 2, Surf_Basis->h / 2 - Surf_Working->h / 2, 0, 0, Surf_Working->w, Surf_Working->h);
	SDL_FreeSurface(Surf_Working);
	Surf_Working = NULL;
	
	for(int i = 1; i < OrganList.size(); i++) {
		Surface::OnDraw(Surf_Basis, OrganList[i]->Surf_Organ, Surf_Basis->w / 2 - OrganList[i]->Surf_Organ->w / 2,  Surf_Basis->h / 2  - OrganList[i]->Surf_Organ->h/2  - (int)(Y - OrganList[i]->Y + Radius/2.0), 0, 0, OrganList[i]->Surf_Organ->w, OrganList[i]->Surf_Organ->h);
	}

	Surf_Entity = Surf_Basis;
}

Animal::~Animal() {
	if(Surf_Body != NULL) SDL_FreeSurface(Surf_Body);
}

Animal::Animal(Animal* SourceAnimal, Brain* SourceBrain): Entity(X, Y, Entity::ENTITY_TYPE_ANIMAL, "../Debug/gfx/animaltest.bmp"), theBrain(SourceBrain) {
	//this->X = SourceAnimal->X;
	//this->Y = SourceAnimal->Y;
	FM = SourceAnimal->FM;
	this->Type = SourceAnimal->Type;

	float SSize = 1.0/WorldSettings::MutationRate;

	float MutationM;
	MutationM = float(75 + rand() % 50)/100.0;

	
	LastColTime = 0;
	if( !(rand() % (int)SSize ) ) { LifeSpan = SourceAnimal->LifeSpan * MutationM; }
	else LifeSpan = SourceAnimal->LifeSpan;

	Age = 0;
	LastReproductionTime = SDL_GetTicks();

	MutationM = float(75 + rand() % 50)/100.0;

	if( !(rand() % (int)SSize ) ) { 
		if( SourceAnimal->MaxMuscleMass * MutationM < 25.0 ) MaxMuscleMass = 25.0; 
		else if( SourceAnimal->MaxMuscleMass * MutationM > 65.0 ) MaxMuscleMass = 65.0;
		else MaxMuscleMass = SourceAnimal->MaxMuscleMass * MutationM; 
	}
	else  MaxMuscleMass = SourceAnimal->MaxMuscleMass * MutationM;

	MutationM = float(75 + rand() % 50)/100.0;

	if( !(rand() % (int)SSize ) ) { 
		if( SourceAnimal->MaxFatMass * MutationM < 1.543 ) MaxFatMass = 1.543;
		else if( SourceAnimal->MaxFatMass * MutationM > 26.080 ) MaxFatMass = 26.080;
		else MaxFatMass = SourceAnimal->MaxFatMass * MutationM; 
	}
	else  MaxFatMass = SourceAnimal->MaxFatMass * MutationM;

	MaxTotalMass = MaxMuscleMass + MaxFatMass;
	RatioFatBurn = (float)0.07;

	MaxEnergy = (float)(37.62 * MaxFatMass + 16.73 * MaxMuscleMass);  //expected about 1091.43 Kilo-Joules, 4.184 kJ = 1 kCal, 9kCal = 1 kg Fat, 4kCal = 1 kg Protein

	theBrain.AnimalMaxEnergy = MaxEnergy;

	CurrentEnergy = SourceAnimal->CurrentEnergy; //change in other function

	CurrentTotalMass = MaxTotalMass;
	CurrentMuscleMass = MaxMuscleMass;
	CurrentFatMass = MaxFatMass;

	Radius = (float)((MaxTotalMass * (1.7/62.0)) * (CurrentTotalMass/(2.0 * MaxTotalMass) + (0.5)) * 32.0 )/2.0; //pixels
	MaxForceOutput = CurrentMuscleMass * 2.5;

	MaxTorqueOutput = MaxForceOutput*Radius/4.0;

	NormalForce = CurrentTotalMass * WorldSettings::Gravity;
	NewVelX = NewVelY = 0;
	DragForceX = DragForceY = 0;
	Torque = AngularAccel = AngularVel = NewAngularVel = Angle = 0;

	int MaxOrganSize = 0;

	Organ* tempOrgan;
	
	for(int i = 0; i < 3; i++) {
		tempOrgan = new Organ(SourceAnimal->OrganList[i], X, Y, Radius, true);
		
		OrganList.push_back(tempOrgan);

		if(OrganList[i]->Surf_Organ != NULL) {
			if(MaxOrganSize < OrganList[i]->Surf_Organ->w) MaxOrganSize = OrganList[i]->Surf_Organ->w;
			if(MaxOrganSize < OrganList[i]->Surf_Organ->h) MaxOrganSize = OrganList[i]->Surf_Organ->h;
		}
	}
	
	MutationM = float(75 + rand() % 50)/100.0;

	if( !(rand() % (int)SSize ) ) {  
		if(int((float)SourceAnimal->Color.R * MutationM) > 255) this->Color.R = 255; 
		else if(int((float)SourceAnimal->Color.R * MutationM) < 0) this->Color.R = 0; 
		else this->Color.R = Uint8((float)SourceAnimal->Color.R * MutationM); 
	}
	else this->Color.R = SourceAnimal->Color.R;

	MutationM = float(75 + rand() % 50)/100.0;

	if( !(rand() % (int)SSize ) ) { 
		if(int((float)SourceAnimal->Color.G * MutationM) > 255) this->Color.G = 255; 
		else if(int((float)SourceAnimal->Color.G * MutationM) < 0) this->Color.G = 0; 
		else this->Color.G = Uint8((float)SourceAnimal->Color.G * MutationM); 
	}
	else this->Color.G = SourceAnimal->Color.G;

	MutationM = float(75 + rand() % 50)/100.0;

	if( !(rand() % (int)SSize ) ) { 
		if(int((float)SourceAnimal->Color.B * MutationM) > 255) this->Color.B = 255; 
		else if(int((float)SourceAnimal->Color.B * MutationM) < 0) this->Color.B = 0; 
		else this->Color.B = Uint8((float)SourceAnimal->Color.B * MutationM); 
	}
	else this->Color.B = SourceAnimal->Color.B;

	/*
	if( !(rand() % (int)SSize ) ) { Color.R = int(SourceAnimal->Color.R * MutationM); }
	else Color.R = SourceAnimal->Color.R;

	if( !(rand() % (int)SSize ) ) { Color.G = int(SourceAnimal->Color.G * MutationM); }
	else Color.G = SourceAnimal->Color.G;

	if( !(rand() % (int)SSize ) ) { Color.B = int(SourceAnimal->Color.B * MutationM); }
	else Color.B = SourceAnimal->Color.B;
	*/

	int SurfWidthScale = (int)((MaxTotalMass * (1.7/62.0)) * (MaxTotalMass/(2.0*MaxTotalMass) + (0.5)) * float(Surf_Entity->w));
	int SurfHeightScale = (int)((MaxTotalMass* (1.7/62.0)) * (MaxTotalMass/(2.0*MaxTotalMass) + (0.5)) * float(Surf_Entity->h));

	Surf_Working = NULL;
	Surf_Body = Surf_Entity;

	//change the following to an exception laters;
	int Count = 0;
	while(Surf_Working == NULL) {
		if(Count > 3) exit(1);
		Surf_Working = Surface::ScaleSurface(Surf_Entity, SurfWidthScale, SurfHeightScale);// SurfWidthScale, SurfHeightScale);
		Count++; 
	}
	Surface::SetSurfColor(Surf_Working, SDL_MapRGB(Surf_Working->format, 0, 0, 0) , SDL_MapRGB(Surf_Working->format, this->Color.R, this->Color.G , this->Color.B));
	Surface::Transparent(Surf_Working, 255, 0, 255);

	// Buggy sometimes crashes :(
	Surf_Basis = SDL_CreateRGBSurface(Surf_Entity->flags, Surf_Entity->w + 2*MaxOrganSize, Surf_Entity->h + 2*MaxOrganSize, Surf_Entity->format->BitsPerPixel, Surf_Entity->format->Rmask, Surf_Entity->format->Gmask, Surf_Entity->format->Bmask, Surf_Entity->format->Amask);
	Surface::SetSurfColor(Surf_Basis, SDL_MapRGB(Surf_Basis->format, 0, 0, 0), SDL_MapRGB(Surf_Basis->format, 255, 0, 255));
	Surface::Transparent(Surf_Basis, 255, 0, 255);
	Surface::OnDraw(Surf_Basis, Surf_Working, Surf_Basis->w / 2 - Surf_Working->w / 2, Surf_Basis->h / 2 - Surf_Working->h / 2, 0, 0, Surf_Working->w, Surf_Working->h);
	SDL_FreeSurface(Surf_Working);
	Surf_Working = NULL;
	
	for(int i = 1; i < OrganList.size(); i++) {
		Surface::OnDraw(Surf_Basis, OrganList[i]->Surf_Organ, Surf_Basis->w / 2 - OrganList[i]->Surf_Organ->w / 2,  Surf_Basis->h / 2  - OrganList[i]->Surf_Organ->h/2  - (int)(Y - OrganList[i]->Y + Radius/2.0), 0, 0, OrganList[i]->Surf_Organ->w, OrganList[i]->Surf_Organ->h);
	}

	Surf_Entity = Surf_Basis;
}

Animal::Animal(Animal* SourceAnimal, Entity* SourceEntity, Brain* SourceBrain): Entity(SourceEntity), theBrain(SourceBrain) {
	//temporary until I write a random number generator
	LastColTime = 0;
	LifeSpan = SourceAnimal->LifeSpan;
	Age = 0;
	LastReproductionTime = SDL_GetTicks();
	FM = 1;

	MaxMuscleMass = SourceAnimal->MaxMuscleMass;
	MaxFatMass = SourceAnimal->MaxFatMass;
	MaxTotalMass = MaxMuscleMass + MaxFatMass;
	RatioFatBurn = (float)0.07;

	MaxEnergy = (float)(37.62 * MaxFatMass + 16.73 * MaxMuscleMass);  //expected about 1091.43 Kilo-Joules, 4.184 kJ = 1 kCal, 9kCal = 1 kg Fat, 4kCal = 1 kg Protein

	theBrain.AnimalMaxEnergy = MaxEnergy;

	CurrentEnergy = SourceAnimal->CurrentEnergy; //change in other function

	CurrentTotalMass = MaxTotalMass;
	CurrentMuscleMass = MaxMuscleMass;
	CurrentFatMass = MaxFatMass;

	Radius = (float)((MaxTotalMass * (1.7/62.0)) * (CurrentTotalMass/(2.0 * MaxTotalMass) + (0.5)) * 32.0)/2.0; //pixels
	MaxForceOutput = CurrentMuscleMass * 2.5;

	MaxTorqueOutput = MaxForceOutput*Radius/4.0;

	NormalForce = CurrentTotalMass * WorldSettings::Gravity;
	NewVelX = NewVelY = 0;
	DragForceX = DragForceY = 0;
	Torque = AngularAccel = AngularVel = NewAngularVel = Angle = 0;

	int MaxOrganSize = 0;

	Organ* tempOrgan;
	
	for(int i = 0; i < 3; i++) {
		tempOrgan = new Organ(SourceAnimal->OrganList[i], X, Y, Radius);
		
		OrganList.push_back(tempOrgan);

		if(OrganList[i]->Surf_Organ != NULL) {
			if(MaxOrganSize < OrganList[i]->Surf_Organ->w) MaxOrganSize = OrganList[i]->Surf_Organ->w;
			if(MaxOrganSize < OrganList[i]->Surf_Organ->h) MaxOrganSize = OrganList[i]->Surf_Organ->h;
		}
	}
	
	
	Color.R = SourceAnimal->Color.R;
	Color.G = SourceAnimal->Color.G;
	Color.B = SourceAnimal->Color.B;
		
	Surf_Basis = Surface::ScaleSurface(SourceEntity->Surf_Basis, SourceEntity->Surf_Basis->w, SourceEntity->Surf_Basis->h);

	Surf_Body = Surface::ScaleSurface(SourceAnimal->Surf_Body, SourceAnimal->Surf_Body->w, SourceAnimal->Surf_Body->h);
	
}

void Animal::OnLoop() {
	CurrentEnergy -= CurrentTotalMass * Entity::JouleDrainPerKilo * FPS::FPSControl->SpeedFactor;
	//if(FM + MR*FPS::FPSControl->SpeedFactor >= 1) FM = 1;
	//else FM += MR*FPS::FPSControl->SpeedFactor;

	//Do Thinking
	if( theBrain.LastProcessingTime + ( 1.0 / ( 1.0 + pow( 2.71828, (double)(OrganList[0]->Attribute1 * 2.1252 /* 1.413*/ /*0.676*/ /*0.21972*/) ) ) ) * 1000.0 < SDL_GetTicks() ) {
		OnThink();
		theBrain.ProcessInputs();

		//Do Actions
		OnBrainOutput();  
		//too many force outputs per loop.  Thinking is fine, but need to put an upper limit on rate of force outputs;


		if(theBrain.MemDelay * 1000.0 + theBrain.LastProcessingTime < SDL_GetTicks()) {
			theBrain.UpdateMemoryList(CurrentEnergy);
		}

		theBrain.UpdateWeights();
	}

	Tile* tempTile = Map::theMap->GetTile((int)(X + (float)Surf_Entity->w/(float)2), (int)(Y + (float)Surf_Entity->h/(float)2));  //this can cause a crash if the coordinates point to a location without a tile, such as off the map.
	if(tempTile == NULL) {
		tempTile = new Tile(0, Tile::TILE_TYPE_DIRT);
		OffMap = true;
	}

	theBrain.InputLayer[9]->Value = (float)tempTile->TypeID;

	ViscousTorque = (WorldSettings::AtmDensity * 6.0 * acos(-1.0))*(Radius/32.0)*abs(AngularVel);
	
	if(abs(AngularVel) < 0.001 && abs(Torque) < NormalForce * (float)tempTile->StaticFrictionC/2.0) Torque = AngularAccel = AngularVel = 0;
	if(abs(AngularVel) > 0) Torque += -(AngularVel/abs(AngularVel)) * NormalForce * (float)tempTile->KineticFrictionC/2.0;
	if(abs(AngularVel) > 0) Torque += -(AngularVel/abs(AngularVel)) * ViscousTorque;
	//Torque = Ftan*R = (moment of Inertia) * (AngularAccel) = ((pi/4)*Radius^4)*AngularAccel.  For a circle
	AngularAccel = (Torque / (atan(1.0)*pow(Radius, 4)));
	NewAngularVel = AngularVel + AngularAccel * FPS::FPSControl->SpeedFactor;
	if((AngularVel > 0 && NewAngularVel < 0) || (AngularVel < 0 && NewAngularVel > 0)) Torque = AngularAccel = NewAngularVel = 0;
	AngularVel = NewAngularVel;
	OnRotate(NewAngularVel);

	// There is image data loss with each rotation call.  This stacks up in a loop very quickly and the original image gets turned into 'soup'.  Must cache original image and use that as a 'template' for each call.
	
	NormalForce = CurrentTotalMass * WorldSettings::Gravity;
	if (VelX != 0) DragForceX = -(VelX / abs(VelX)) * WorldSettings::AtmDensity * (pow(VelX, 2)*(Radius/32));
	else DragForceX = 0;
	if (VelY != 0) DragForceY = -(VelY / abs(VelY)) * WorldSettings::AtmDensity * (pow(VelY, 2)*(Radius/32));
	else DragForceY = 0;

	if(abs(VelX) < 0.001 && abs(NetForceX) < NormalForce * tempTile->StaticFrictionC) {
		NetForceX = 0;
		AccelX = 0;
		VelX = 0;
	}
	else if(abs(VelX) > 0) {
			NetForceX += -(VelX/abs(VelX)) * NormalForce * tempTile->KineticFrictionC;
			NetForceX += DragForceX; 
		}
	
	
	if((abs(VelY) <  0.001) && abs(NetForceY) < NormalForce * tempTile->StaticFrictionC) {
		NetForceY = 0;
		AccelY = 0;
		VelY = 0;
	}
	else if(abs(VelY) > 0) {
				NetForceY += -(VelY/abs(VelY)) * NormalForce * tempTile->KineticFrictionC;
				NetForceY += DragForceY; 
	}

	if(OffMap) {
		delete tempTile;
		OffMap = false;
	}
	
	AccelX = (NetForceX / CurrentTotalMass);
	AccelY = (NetForceY / CurrentTotalMass);
	NewVelX = VelX + AccelX * FPS::FPSControl->SpeedFactor;
	NewVelY = VelY + AccelY * FPS::FPSControl->SpeedFactor;

	if((VelX >0 && NewVelX < 0) || (VelX < 0 && NewVelX >0)) NetForceX = AccelX = NewVelX = 0;// this will need to be done first.
	if((VelY >0 && NewVelY < 0) || (VelY < 0 && NewVelY >0)) NetForceY = AccelY = NewVelY = 0;

	VelX = NewVelX;
	VelY = NewVelY;

	OnMove(NewVelX, NewVelY);

	//Change Body Shape  in a later version
	/*if(FPS::FPSControl->OldTime + 1000 < SDL_GetTicks()) {
		CurrentTotalMass = MaxTotalMass * (CurrentEnergy / MaxEnergy);
		CurrentMuscleMass = CurrentTotalMass * (MaxMuscleMass/MaxTotalMass);
		CurrentFatMass = CurrentTotalMass * (MaxFatMass/MaxTotalMass);

		MaxForceOutput = CurrentMuscleMass * 2.15;
		MaxTorqueOutput = MaxForceOutput*Radius;

		Radius = (float)((MaxTotalMass * (1.7/62)) * (CurrentTotalMass/(2*MaxTotalMass) + (0.6)) * 32)/2; //pixels
	}*/

	if(CurrentEnergy <= 0) {
		CurrentEnergy = 0;
		Dead = true;
		Corpse* tempPointer = new Corpse(X, Y);
		
		int SurfWidthScale = (int)((MaxTotalMass * (1.7/62)) * (MaxTotalMass/(2*MaxTotalMass) + (0.5)) * tempPointer->Surf_Entity->w);
		int SurfHeightScale = (int)((MaxTotalMass* (1.7/62)) * (MaxTotalMass/(2*MaxTotalMass) + (0.5)) * tempPointer->Surf_Entity->h);

		tempPointer->Surf_Working = Surface::ScaleSurface(tempPointer->Surf_Entity, SurfWidthScale, SurfHeightScale);// SurfWidthScale, SurfHeightScale);

		Surface::Transparent(tempPointer->Surf_Working, 255, 0, 255);
		SDL_FreeSurface(tempPointer->Surf_Entity);
		tempPointer->Surf_Entity = tempPointer->Surf_Working;

		tempPointer->X = X + Surf_Entity->w/2 - tempPointer->Surf_Entity->w/2;
		tempPointer->Y = Y + Surf_Entity->h/2 - tempPointer->Surf_Entity->h/2;

		Entity::EntityList.push_back(tempPointer);
	}

	//make world spherical
	if(X + (Surf_Entity->w - Radius)/2 + 2*Radius < 0) X = X + 2*Radius + Map::theMap->Width;
	else if(X + (Surf_Entity->w - Radius)/2 > Map::theMap->Width) X = X - 2*Radius - Map::theMap->Width;
	if(Y + (Surf_Entity->h - Radius)/2 + 2*Radius < 0) Y = Y + 2*Radius + Map::theMap->Height;
	else if(Y + (Surf_Entity->h - Radius)/2 > Map::theMap->Height) Y = Y - 2*Radius - Map::theMap->Height;
}

void Animal::OnRender(SDL_Surface* Surf_Display) {
	//if(FPS::FPSControl->OldTime + 1000 < SDL_GetTicks()) {
			Surf_Working = Surface::RotateSurface(Surf_Basis, Angle);
			Surf_Entity = Surf_Working;
			Surface::Transparent(Surf_Entity, 255, 0, 255);
	//	}
	Surface::OnDraw(Surf_Display, Surf_Entity, (int)X - Camera::theCamera->X, (int)Y - Camera::theCamera->Y, 0, 0, Surf_Entity->w, Surf_Entity->h);
	Surf_Entity = Surf_Basis;
	SDL_FreeSurface(Surf_Working);
	Surf_Working = NULL;
	//for(int i = 1; i < OrganList.size(); i++) OrganList[i]->OnRender(Surf_Display);
}

void Animal::OnForceOutput(float FX, float FY) {
	NetForceX += FX;
	NetForceY += FY;
}

void Animal::OnTorqueOutput(float T) { 
	Torque += T;
}

void Animal::OnMove(float MoveX, float MoveY) {
	
	MoveX *= FPS::FPSControl->SpeedFactor * 32;
	MoveY *= FPS::FPSControl->SpeedFactor * 32;

	if(PosValid(X + MoveX, Y + MoveY)) {  //PosValid evaluating false for a single Animal seems to halt movement for ALL animals.
		X += MoveX;
		Y += MoveY;

		for(int i = 1; i < OrganList.size(); i++) {
			OrganList[i]->X += MoveX;
			OrganList[i]->Y += MoveY;
		}
	}
	
}

void Animal::OnRotate(float AngularMove) {
	AngularMove *= FPS::FPSControl->SpeedFactor;
	Angle += AngularMove;
	if( Angle >= acos(0.0) * float(4) ) Angle -= acos(0.0) * float(4);
	if( Angle < 0 ) Angle += acos(0.0) * float(4);
	for(int i = 1; i < OrganList.size(); i++) OrganList[i]->Angle += AngularMove;
}

void Animal::OnThink() {
	//Get sense data.  Based off of organ attributes and locations.
	float SightRed, SightGreen, SightBlue, SightDistance, SightAngle, TargetEnergy = 0;  //Eye Attribute2 gives 5 "meter" per 1 point.  1 "meter" = 32 pixels at 1x zoom.
	//Nodes 13 - 23 will contain sight data pertaining to closest plant and Tile respectively.
	//inputs above go to nodes 0, 1, 2, 3, 4 respectively;
	//Nodes 5, 6, 7 are handled in collision code
	//TargetEnergy into the 10th node;
	//CurrentEnergy into the 11th node;
	float LowestT, Discrim, L1x, L1y, L2x, L2y, Cx, Cy, Dx, Dy, T1, T2, TargRad;
	float LoopT = 2;
	LowestT = 2;
	int BestIndex = -1;

	L1x = -Radius * sin(Angle) + X + (float)Surf_Entity->w/(float)2;
	L1y = -Radius * cos(Angle) + Y + (float)Surf_Entity->h/(float)2;
	L2x = -(Radius + OrganList[2]->Attribute2 * 160.0) * sin(Angle) + X + (float)Surf_Entity->w/(float)2;
	L2y = -(Radius + OrganList[2]->Attribute2 * 160.0) * cos(Angle) + Y + (float)Surf_Entity->h/(float)2;
	Dx = L2x - L1x;
	Dy = L2y - L1y;

	if(BestIndex == -1) {
		for(int i = 0; i < AnimalList.size(); i++) {
			if(AnimalList[i] != NULL && AnimalList[i] != this) {
				TargRad = AnimalList[i]->Radius;
				Cx = AnimalList[i]->X + (float)AnimalList[i]->Surf_Entity->w/(float)2;
				Cy = AnimalList[i]->Y + (float)AnimalList[i]->Surf_Entity->h/(float)2;
				Discrim = 4 * ( pow((L1x * Dx + L1y * Dy - Cx * Dx - Cy * Dy), 2) ) - 4 * (Dx * Dx + Dy * Dy) * ( pow( (L1x - Cx) , 2) + pow( (L1y - Cy) , 2 ) - pow(TargRad, 2)); 
				LoopT = 2;

				if(Discrim >= 0) { 
					T1 = ( (1/(2 * ( Dx * Dx + Dy * Dy ) ) * ( -2 * (  ( L1x * Dx + L1y * Dy ) - (Cx * Dx + Cy * Dy) ) + sqrt(Discrim) ) ) ) ;
					T2 = ( (1/(2 * ( Dx * Dx + Dy * Dy ) ) * ( -2 * (  ( L1x * Dx + L1y * Dy ) - (Cx * Dx + Cy * Dy) ) - sqrt(Discrim) ) ) ) ;

					if( 0 <= T1 && T1 <= 1 && 0 <= T2 && T2 <= 1 ) {
						if ( T1 < T2 && T1 < LoopT) LoopT = T1;
						else if ( T2 < LoopT) LoopT = T2;
					}
					else if( 0 <= T1 && T1 <= 1 && T1 < LoopT) LoopT = T1;
					else if ( 0 <= T2 && T2 <= 1 && T2 < LoopT) LoopT = T2;
				}

				if( LoopT < LowestT ) {
					LowestT = LoopT;
					BestIndex = i;
				}
			}
		}

		if(BestIndex >= 0) {
			SightRed = (float)AnimalList[BestIndex]->Color.R;
			SightGreen = (float)AnimalList[BestIndex]->Color.G;
			SightBlue = (float)AnimalList[BestIndex]->Color.B;
			SightDistance = sqrt( pow( (L2x - L1x) * LowestT + L1x, 2 ) +  pow( (L2y - L1y) * LowestT + L1y, 2 ) );
			SightAngle = Angle;
			TargetEnergy = AnimalList[BestIndex]->CurrentEnergy;
		}
		else {
			SightRed = (float)-1;
			SightGreen = (float)-1;
			SightBlue = (float)-1;
			SightDistance = (float)-1;;
			SightAngle = (float)-1;
			TargetEnergy = (float)-1;
		}
	}

	theBrain.InputLayer[0]->Value = SightRed;
	theBrain.InputLayer[1]->Value = SightGreen;
	theBrain.InputLayer[2]->Value = SightBlue;
	theBrain.InputLayer[3]->Value = SightDistance;
	theBrain.InputLayer[4]->Value = SightAngle;

	BestIndex = -1;
	

	float PX0, PX1, PY0, PY1, VarX, VarY; //plant corners are (PX0, PY0) (PX0, PY1) (PX1, PY0) (PX1, PY1)
	if(BestIndex == -1) {
		for(int i = 0; i < Plant::PlantList.size(); i++) {
			PX0 = Plant::PlantList[i]->X; 
			PY0 = Plant::PlantList[i]->Y;
			PX1 = Plant::PlantList[i]->X + Plant::PlantList[i]->PlantFrameList[0]->w;
			PY1 = Plant::PlantList[i]->Y + Plant::PlantList[i]->PlantFrameList[0]->h;
			

			if(Dy != 0) {
				LoopT = (PY0 - L1y) / Dy;
				VarX = Dx * LoopT + L1x; 
				if( PX0 <= VarX && VarX <= PX1 && 0 <= LoopT && LoopT <= 1) {
					if (LoopT < LowestT) {
						LowestT = LoopT;
						BestIndex = i;
					}
				}

				LoopT = (PY1 - L1y) / Dy;
				VarX = Dx * LoopT + L1x; 
				if( PX0 <= VarX && VarX <= PX1 && 0 <= LoopT && LoopT <= 1) {
					if (LoopT < LowestT) {
						LowestT = LoopT;
						BestIndex = i;
					}
				}
			}

			if(Dx != 0) {
				LoopT = (PX0 - L1x) / Dx;
				VarY = Dy * LoopT + L1y; 
				if( PY0 <= VarY && VarY <= PY1 && 0 <= LoopT && LoopT <= 1) {
					if (LoopT < LowestT) {
						LowestT = LoopT;
						BestIndex = i;
					}
				}

				LoopT = (PX1 - L1x) / Dx;
				VarY = Dy * LoopT + L1y; 
				if( PY0 <= VarY && VarY <= PY1 && 0 <= LoopT && LoopT <= 1) {
					if (LoopT < LowestT) {
						LowestT = LoopT;
						BestIndex = i;
					}
				}
			}

		}

		if(BestIndex >= 0) {
			SightRed = (float)Plant::PlantList[BestIndex]->Color.R;
			SightGreen = (float)Plant::PlantList[BestIndex]->Color.G;
			SightBlue = (float)Plant::PlantList[BestIndex]->Color.B;
			SightDistance = sqrt( pow( (L2x - L1x) * LowestT + L1x, 2 ) +  pow( (L2y - L1y) * LowestT + L1y, 2 ) );
			SightAngle = Angle;
			TargetEnergy = Plant::PlantList[BestIndex]->CurrentEnergy;
		}
		else {
			SightRed = (float)-1;
			SightGreen = (float)-1;
			SightBlue = (float)-1;
			SightDistance = (float)-1;;
			SightAngle = (float)-1;
			TargetEnergy = (float)-1;
		}
	}

	theBrain.InputLayer[13]->Value = SightRed;
	theBrain.InputLayer[14]->Value = SightGreen;
	theBrain.InputLayer[15]->Value = SightBlue;
	theBrain.InputLayer[16]->Value = SightDistance;
	theBrain.InputLayer[17]->Value = SightAngle;

	BestIndex = -1;
	

	if(BestIndex == -1) {
		Tile* tempTile = NULL;
		tempTile = Map::theMap->GetTile(L1x, L1y);

		if(tempTile != NULL && tempTile != 0) {
			SightRed = (float)tempTile->Color.R;
			SightGreen = (float)tempTile->Color.G;
			SightBlue = (float)tempTile->Color.B;
			SightDistance = Radius;
			SightAngle = Angle;
		}
		else { //in case off-map
			SightRed = (float)144;
			SightGreen = (float)102;
			SightBlue = (float)72;
			SightDistance = Radius;
			SightAngle = Angle;
		}
	}


	theBrain.InputLayer[18]->Value = SightRed;
	theBrain.InputLayer[19]->Value = SightGreen;
	theBrain.InputLayer[20]->Value = SightBlue;
	theBrain.InputLayer[21]->Value = SightDistance;
	theBrain.InputLayer[22]->Value = SightAngle;

	theBrain.InputLayer[8]->Value = (float)(SDL_GetTicks() - LastColTime);
	//Node 9 handled in MainLoop
	//Node 10 in Collisions

	theBrain.InputLayer[11]->Value = TargetEnergy;

	float LastCEnergy = theBrain.InputLayer[12]->Value;

	theBrain.InputLayer[12]->Value = CurrentEnergy;

	theBrain.InputLayer[23]->Value = VelX; //sqrt( pow(VelX, 2) + pow(VelY, 2) );
	theBrain.InputLayer[24]->Value = VelY; //acos( ( VelX * -sin(Angle) + VelY * -cos(Angle) ) / ( sqrt ( pow(VelX, 2) + pow(VelY, 2) ) ) );
	theBrain.InputLayer[25]->Value = AngularVel;
	theBrain.InputLayer[26]->Value = ( CurrentEnergy - LastCEnergy) / ( float(SDL_GetTicks()) - theBrain.LastProcessingTime );

	theBrain.LastProcessingTime = SDL_GetTicks(); 
}

void Animal::OnBrainOutput() {
	//OutputLayer[0] is "type of action" node.  //n = number of possible actions - 1 = OutputLayer.size() - 1;
	float n = theBrain.OutputLayer.size() - 1;
	bool Reproduced = false;
	/*
	if(n + 1 == 3) {
		float Type = theBrain.OutputLayer[0]->SigFunction(theBrain.OutputLayer[0]->Value);
		float A = theBrain.OutputLayer[1]->SigFunction(theBrain.OutputLayer[1]->Value);
		float B = theBrain.OutputLayer[2]->SigFunction(theBrain.OutputLayer[2]->Value);

			if( Type < -1 + 2/n  ) {
				OnTorqueOutput( MaxTorqueOutput * A );
				//OnTorqueOutput( MaxTorqueOutput * (A + B)/2 );
			}
			else if (Type < 2 -1 + 4/n ) {
				OnForceOutput(MaxForceOutput * B * -sin(Angle), MaxForceOutput * B * -cos(Angle));
				
				float OutputX, OutputY, DAngle, DMultiplier;
				if( A == 0 && B == 0) OnForceOutput(0, 0);
				else {
					DAngle = acos( (1 / sqrt( pow(  MaxForceOutput * (A / (abs(A) + abs(B))), 2 ) + pow( MaxForceOutput * (B / (abs(A) + abs(B))), 2 ) ) ) * (  MaxForceOutput * (A / (abs(A) + abs(B)) ) * -sin(Angle) +  MaxForceOutput * (B / (abs(A) + abs(B)) ) * -cos(Angle) ) );
					DMultiplier = ( 1 - ( 3.0 / ( 4.0 * 2.0 * acos((double)0) ) ) * abs(DAngle) ); //if parallel with facing angle then 1, if opposite of facing angle then 0.25
					OutputX =  MaxForceOutput * (A / (abs(A) + abs(B))) * DMultiplier;
					OutputY =  MaxForceOutput * (B / (abs(A) + abs(B))) * DMultiplier;
		
					OnForceOutput ( OutputX , OutputY );
				}
			}
		*/
	
	//Want to add a "Muscle Fatigue" system.  Will have Fatigue Multiplier, FM, s.t. 0 <= FM <= 1.  The Force & Torque inputs will be scaled by this FM.  Force & Torque functions will increase FM based on input and two static variables:
	// Muscle Fatigue per Newton Output variable, MFPN, and Muscle Recovery per Second, MRPS;  Each loop will increase FM by MRPS * SpeedFactor;  ForceOutput will decrease FM by  MFPS * (Force Output Magnitude) * SpeedFactor;
	// This will help to limit constant acceleration while allowing for the option of many many more brain-thinking loops per second of real time (as that is currently limited to limit acceleration);
	if(n + 1 == 4) {
		float A = theBrain.OutputLayer[1]->SigFunction(theBrain.OutputLayer[1]->Value);
		float B = theBrain.OutputLayer[2]->SigFunction(theBrain.OutputLayer[2]->Value);
		float C = theBrain.OutputLayer[3]->SigFunction(theBrain.OutputLayer[3]->Value);

		float Type = theBrain.OutputLayer[0]->SigFunction(theBrain.OutputLayer[0]->Value);

		if(theBrain.OutputLayer[3]->ActivationFunction()  && sqrt(pow(VelX, 2) + pow(VelY, 2)) < 0.5) {
			if(C > 0) {
				if(OnReproduce(NULL, abs(C))) Reproduced = true;
			}
		}

		if(!(Reproduced)) {

			if( Type < float(-1) + float(2)/(n - float(1))  ) {
				OnTorqueOutput( MaxTorqueOutput * A );
				CurrentEnergy -= (MaxTorqueOutput * abs(A)) * (Entity::EDPN) * 2.0/Radius;

				/*
					OnTorqueOutput( MaxTorqueOutput * A * FM);
					if(FM - abs(A) * Entity::MFG / Radius <= 0) FM = 0;
					else FM -= abs(A) * Entity::MFG / Radius;
					*/
					//OnTorqueOutput( MaxTorqueOutput * (A + B)/2 );
				}
				else if (Type < float(-1) + float(4)/(n - float(1))  ) {
					//OnForceOutput(MaxForceOutput * B * -sin(Angle), MaxForceOutput * B * -cos(Angle));
				
					float OutputX, OutputY, DAngle, DMultiplier;
					if( A == 0 && B == 0) OnForceOutput(0, 0);
					else {
						DAngle = acos( (1 / sqrt( pow(  MaxForceOutput * (A / (abs(A) + abs(B))), 2 ) + pow( MaxForceOutput * (B / (abs(A) + abs(B))), 2 ) ) ) * (  MaxForceOutput * (A / (abs(A) + abs(B)) ) * -sin(Angle) +  MaxForceOutput * (B / (abs(A) + abs(B)) ) * -cos(Angle) ) );
						DMultiplier = ( 1 - ( 3.0 / ( 4.0 * 2.0 * acos((double)0) ) ) * abs(DAngle) ); //if parallel with facing angle then 1, if opposite of facing angle then 0.25
						OutputX =  MaxForceOutput * (A / (abs(A) + abs(B))) * DMultiplier;
						OutputY =  MaxForceOutput * (B / (abs(A) + abs(B))) * DMultiplier;

						OnForceOutput ( OutputX, OutputY );
						
						CurrentEnergy -= sqrt( pow(OutputX, 2) + pow(OutputY, 2) ) * Entity::EDPN;

						/*
						OnForceOutput ( OutputX * FM, OutputY * FM );

						if(FM - sqrt( (pow(A, 2) + pow(B, 2) ) / pow((abs(A) + abs(B)), 2) )* Entity::MFG <= 0) FM = 0;
						else FM -= sqrt( (pow(A, 2) + pow(B, 2) ) / pow((abs(A) + abs(B)), 2) ) * Entity::MFG;
						*/
					}
				}

		 }
	}
	
	else if(n + 1 == 3) {
		float A = theBrain.OutputLayer[0]->SigFunction(theBrain.OutputLayer[0]->Value);
		float B = theBrain.OutputLayer[1]->SigFunction(theBrain.OutputLayer[1]->Value);
		float C = theBrain.OutputLayer[2]->SigFunction(theBrain.OutputLayer[2]->Value);
		
		if(theBrain.OutputLayer[2]->ActivationFunction()  && sqrt(pow(VelX, 2) + pow(VelY, 2)) < 0.5) {
			if(C > 0) {
				if(OnReproduce(NULL, abs(C))) Reproduced = true;
			}
		}

		if(!(Reproduced)) {
			OnTorqueOutput( MaxTorqueOutput * A);
			CurrentEnergy -= (MaxTorqueOutput * abs(A)) * (Entity::EDPN) * 2.0/Radius;

			OnForceOutput( MaxForceOutput * abs(B) * -sin(Angle), MaxForceOutput * B * -cos(Angle) );
			CurrentEnergy -= MaxForceOutput * abs(B) * Entity::EDPN;

			/*
			OnTorqueOutput( MaxTorqueOutput * A * FM);

			if(FM - abs(A)*Entity::MFG / Radius <= 0) FM = 0;
			else FM -= abs(A) * Entity::MFG / Radius;

			
			OnForceOutput(MaxForceOutput * B * -sin(Angle) * FM, MaxForceOutput * B * -cos(Angle) * FM);

			if(FM - abs(B)*Entity::MFG <= 0) FM = 0;
			else FM -= abs(B)*Entity::MFG;
			*/
		}
	}
}

bool Animal::AnimalCollides(Animal* TargetAnimal, float NewX, float NewY) {
	if(TargetAnimal == this) return false;
	//if(TargetAnimal->Surf_Entity == NULL || TargetAnimal == NULL || TargetAnimal == this) return false;
	EntityCol tempEntityCol;

	float Discrim, L1x, L1y, L2x, L2y, Cx, Cy, Dx, Dy, T1, T2, TargRad;
	L1x = -Radius * sin(Angle) + X + (float)Surf_Entity->w/(float)2;
	L1y = -Radius * cos(Angle) + Y + (float)Surf_Entity->h/(float)2;
	L2x = -(Radius + OrganList[1]->Height) * sin(Angle) + X + (float)Surf_Entity->w/(float)2;
	L2y = -(Radius + OrganList[1]->Height) * cos(Angle) + Y + (float)Surf_Entity->h/(float)2;
	Cx = TargetAnimal->X + (float)TargetAnimal->Surf_Entity->w/(float)2;
	Cy = TargetAnimal->Y + (float)TargetAnimal->Surf_Entity->h/(float)2;
	Dx = L2x - L1x;
	Dy = L2y - L1y;
	TargRad = TargetAnimal->Radius;
	Discrim = 4 * ( pow((L1x * Dx + L1y * Dy - Cx * Dx - Cy * Dy), 2) ) - 4 * (Dx * Dx + Dy * Dy) * ( pow( (L1x - Cx) , 2) + pow( (L1y - Cy) , 2 ) - pow(TargRad, 2)); 
	

	if(Discrim >= 0) { 
		T1 = ( (1/(2 * ( Dx * Dx + Dy * Dy ) ) * ( -2 * (  ( L1x * Dx + L1y * Dy ) - (Cx * Dx + Cy * Dy) ) + sqrt(Discrim) ) ) ) ;
		T2 = ( (1/(2 * ( Dx * Dx + Dy * Dy ) ) * ( -2 * (  ( L1x * Dx + L1y * Dy ) - (Cx * Dx + Cy * Dy) ) - sqrt(Discrim) ) ) ) ;
		if( 0 <= T1 && T1 <= 1 ) {
			tempEntityCol.EntityA = this;
			tempEntityCol.EntityB = TargetAnimal;
			EntityCol::EntityColList.push_back(tempEntityCol);

			LastColTime = (float)SDL_GetTicks();
			theBrain.InputLayer[5]->Value = sqrt( pow(Dx , 2) + pow(Dy , 2) );
			theBrain.InputLayer[6]->Value = acos( -(Dx * sin(Angle) + Dy * cos(Angle))/(theBrain.InputLayer[5]->Value) );
			theBrain.InputLayer[7]->Value = 0;
			theBrain.InputLayer[10]->Value = 30; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal Body, 3 ~ Spike
		}
		else if ( 0 <= T2 && T2 <= 1 ) {
			tempEntityCol.EntityA = this;
			tempEntityCol.EntityB = TargetAnimal;
			EntityCol::EntityColList.push_back(tempEntityCol);

			LastColTime = (float)SDL_GetTicks();
			theBrain.InputLayer[5]->Value = sqrt( pow(Dx , 2) + pow(Dy , 2) );
			theBrain.InputLayer[6]->Value = acos( -(Dx * sin(Angle) + Dy * cos(Angle))/(theBrain.InputLayer[5]->Value) );
			theBrain.InputLayer[7]->Value = 0;
			theBrain.InputLayer[10]->Value = 30; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal, 3 ~ Spike
		}
	}

	float CenterDifferenceX, CenterDifferenceY, CenterDifferenceTotal;
	CenterDifferenceX = (TargetAnimal->X + (float)TargetAnimal->Surf_Entity->w/(float)2) - (NewX + (float)Surf_Entity->w/(float)2);
	CenterDifferenceY = (TargetAnimal->Y + (float)TargetAnimal->Surf_Entity->h/(float)2) - (NewY + (float)Surf_Entity->h/(float)2);
	CenterDifferenceTotal = sqrt(pow(CenterDifferenceX, 2) + pow(CenterDifferenceY, 2));

	if(CenterDifferenceTotal < Radius + TargetAnimal->Radius) {
		tempEntityCol.EntityA = this;
		tempEntityCol.EntityB = TargetAnimal;
		tempEntityCol.BodyCol = true;
		EntityCol::EntityColList.push_back(tempEntityCol);

		LastColTime = (float)SDL_GetTicks();
		theBrain.InputLayer[5]->Value = sqrt( pow(CenterDifferenceX , 2) + pow(CenterDifferenceY , 2) );
		theBrain.InputLayer[6]->Value = acos( -(CenterDifferenceX * sin(Angle) + CenterDifferenceY * cos(Angle))/(CenterDifferenceTotal) );
		theBrain.InputLayer[7]->Value = 0;
		theBrain.InputLayer[10]->Value = 20; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal, 3 ~ Spike

		return true;
	}

	theBrain.InputLayer[10]->Value = 0; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal, 3 ~ Spike

	return false;
}

bool Animal::PlantCollides(Plant* TargetPlant) {
	if(Plant::PlantFrameList[0] == NULL || TargetPlant == NULL) return false;
	float AnimalCenterX = X + Surf_Entity->w/2;
	float AnimalCenterY = Y + Surf_Entity->w/2; 
	float ClosestPlantX, ClosestPlantY;
	bool ClosestPlantInit = false;

	if(AnimalCenterX <= TargetPlant->X && AnimalCenterY <= TargetPlant->Y) {
		ClosestPlantX = TargetPlant->X;
		ClosestPlantY = TargetPlant->Y;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX < TargetPlant->X + Plant::PlantFrameList[0]->w && AnimalCenterY <= TargetPlant->Y ) {
		ClosestPlantX = AnimalCenterX;
		ClosestPlantY = TargetPlant->Y;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX >= TargetPlant->X + Plant::PlantFrameList[0]->w && AnimalCenterY <= TargetPlant->Y ) {
		ClosestPlantX = TargetPlant->X + Plant::PlantFrameList[0]->w;
		ClosestPlantY = TargetPlant->Y;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX <= TargetPlant->X && AnimalCenterY >= TargetPlant->Y + Plant::PlantFrameList[0]->h ) {
		ClosestPlantX = TargetPlant->X;
		ClosestPlantY = TargetPlant->Y + Plant::PlantFrameList[0]->h;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX < TargetPlant->X + Plant::PlantFrameList[0]->w && AnimalCenterY >= TargetPlant->Y + Plant::PlantFrameList[0]->h) {
		ClosestPlantX = AnimalCenterX;
		ClosestPlantY = TargetPlant->Y + Plant::PlantFrameList[0]->h;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX >= TargetPlant->X + Plant::PlantFrameList[0]->w && AnimalCenterY >= TargetPlant->Y + Plant::PlantFrameList[0]->h) {
		ClosestPlantX = TargetPlant->X + Plant::PlantFrameList[0]->w;
		ClosestPlantY = TargetPlant->Y + Plant::PlantFrameList[0]->h;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX <= TargetPlant->X && AnimalCenterY >= TargetPlant->Y && AnimalCenterY <= TargetPlant->Y + Plant::PlantFrameList[0]->h) {
		ClosestPlantX = TargetPlant->X;
		ClosestPlantY = AnimalCenterY;
		ClosestPlantInit = true;
	}
	else if( AnimalCenterX >= TargetPlant->X + Plant::PlantFrameList[0]->w && AnimalCenterY >= TargetPlant->Y && AnimalCenterY <= TargetPlant->Y + Plant::PlantFrameList[0]->h ) {
		ClosestPlantX = TargetPlant->X + Plant::PlantFrameList[0]->w;
		ClosestPlantY = AnimalCenterY;
		ClosestPlantInit = true;
	}
	else if ( AnimalCenterX >= TargetPlant->X && AnimalCenterX <= TargetPlant->X + Plant::PlantFrameList[0]->w && AnimalCenterY >= TargetPlant->Y && AnimalCenterY <= TargetPlant->Y + Plant::PlantFrameList[0]->h) {
		EntityCol tempEntityCol;
		tempEntityCol.EntityA = this;
		tempEntityCol.EntityB = TargetPlant;
		EntityCol::EntityColList.push_back(tempEntityCol);

		float Dx, Dy;
		Dx = AnimalCenterX - (TargetPlant->X + (float)Plant::PlantFrameList[0]->w/2);
		Dy = AnimalCenterY - (TargetPlant->Y + (float)Plant::PlantFrameList[0]->h/2);
		LastColTime = (float)SDL_GetTicks();
		theBrain.InputLayer[5]->Value = sqrt( pow(Dx , 2) + pow(Dy , 2) );
		theBrain.InputLayer[6]->Value = acos( -(Dx * sin(Angle) + Dy * cos(Angle))/(theBrain.InputLayer[5]->Value) );
		theBrain.InputLayer[7]->Value = 0;
		theBrain.InputLayer[10]->Value = 10; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal, 3 ~ Spike

		return true;
	}

	if(ClosestPlantInit) if(sqrt(pow(ClosestPlantX - AnimalCenterX, 2) + pow(ClosestPlantY - AnimalCenterY, 2)) < Radius*(0.75)) { //Radius alone might be too sensitive.
		EntityCol tempEntityCol;
		tempEntityCol.EntityA = this;
		tempEntityCol.EntityB = TargetPlant;
		EntityCol::EntityColList.push_back(tempEntityCol);
		
		float Dx, Dy;
		Dx = AnimalCenterX - (float)ClosestPlantX;
		Dy = AnimalCenterY - (float)ClosestPlantY;
		LastColTime = (float)SDL_GetTicks();
		theBrain.InputLayer[5]->Value = sqrt( pow(Dx , 2) + pow(Dy , 2) );
		theBrain.InputLayer[6]->Value = acos( -(Dx * sin(Angle) + Dy * cos(Angle))/(theBrain.InputLayer[5]->Value) );
		theBrain.InputLayer[7]->Value = 0;
		theBrain.InputLayer[10]->Value = 10; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal, 3 ~ Spike

		return true;
	}

	theBrain.InputLayer[10]->Value = 0; // 0 ~ nothing, 1 ~ plant, 2 ~ Animal, 3 ~ Spike

	return false;
}

bool Animal::PosValid(float NewX, float NewY) {
	for(int i = 0; i < Animal::AnimalList.size(); i++) {
		if(AnimalCollides(Animal::AnimalList[i], NewX, NewY)) return false;
	}

	return true;
}


bool Animal::OnCollision(Entity* TargetEntity, bool BodyCol) {
	if(TargetEntity->Type == Entity::ENTITY_TYPE_PLANT) {
		if(CurrentEnergy < MaxEnergy) CurrentEnergy += 30*FPS::FPSControl->SpeedFactor;  //later add a check if plant has less than 50*speedfactor energy
		theBrain.InputLayer[7]->Value = -30*FPS::FPSControl->SpeedFactor;
		return true;
	}
	else if(TargetEntity->Type == Entity::ENTITY_TYPE_ANIMAL) {
		OnCollisionAnimal((Animal*)TargetEntity, BodyCol);
		//return true;
		return false; //dont want it to calculate twice, although might need to change this to true.
	}
	else return false;
}

void Animal::OnCollisionAnimal(Animal* TargetAnimal, bool BodyCol) { 
	if(!BodyCol) { 
		float Discrim, L1x, L1y, L2x, L2y, Cx, Cy, Dx, Dy, T1, T2, TargRad, ColX, ColY;
		bool ValidT = false;
		bool MakeBlood = true;
		L1x = -Radius * sin(Angle) + X + float(Surf_Entity->w)/float(2);
		L1y = -Radius * cos(Angle) + Y + float(Surf_Entity->h)/float(2);
		L2x = -(Radius + float(OrganList[1]->Height)) * sin(Angle) + X + float(Surf_Entity->w)/float(2);
		L2y = -(Radius + float(OrganList[1]->Height)) * cos(Angle) + Y + float(Surf_Entity->h)/float(2);
		Cx = TargetAnimal->X + float(TargetAnimal->Surf_Entity->w)/float(2);
		Cy = TargetAnimal->Y + float(TargetAnimal->Surf_Entity->h)/float(2);
		Dx = L2x - L1x;
		Dy = L2y - L1y;
		TargRad = TargetAnimal->Radius;
		Discrim = 4 * ( pow((L1x * Dx + L1y * Dy - Cx * Dx - Cy * Dy), 2) ) - 4 * (Dx * Dx + Dy * Dy) * ( pow( (L1x - Cx) , 2) + pow( (L1y - Cy) , 2 ) - pow(TargRad, 2)); 
		

		if(Discrim >= 0) { 
			T1 = ( (1/(2 * ( Dx * Dx + Dy * Dy ) ) * ( -2 * ( ( L1x * Dx + L1y * Dy ) - (Cx * Dx + Cy * Dy) ) + sqrt(Discrim) ) ) ) ;
			T2 = ( (1/(2 * ( Dx * Dx + Dy * Dy ) ) * ( -2 * ( ( L1x * Dx + L1y * Dy ) - (Cx * Dx + Cy * Dy) ) - sqrt(Discrim) ) ) ) ;

			if( 0 <= T1 && T1 <= 1 ) {
				ColX = T1 * Dx + L1x;
				ColY = T1 * Dy + L1y;
				ValidT = true;
			}
			else if ( 0 <= T2 && T2 <= 1 ) {
				ColX = T2 * Dx + L1x;
				ColY = T2 * Dy + L1y;
				ValidT = true;
			}

			if(ValidT) {
				for(int i = 0; i < Entity::EntityList.size(); i++) {
					if(Entity::EntityList[i]->Type == Entity::ENTITY_TYPE_MISC) {
						if( !(( ColX < Entity::EntityList[i]->X - 16 ) || ( ColX > Entity::EntityList[i]->X + 16 ) || ( ColY < Entity::EntityList[i]->Y - 16 )  || ( ColY > Entity::EntityList[i]->Y + 16 )) ) {
							MakeBlood = false;
						}
					}
				}

				float Vn, VnTarg, UNX, UNY, MagNetV, Damage;
				UNX = (Dx / sqrt(pow(Dx, 2) + pow(Dy, 2)));
				UNY = (Dy / sqrt(pow(Dx, 2) + pow(Dy, 2)));
				Vn = (VelX * UNX) + (VelY * UNY);  //magnitude of vector component along line through centers of entities
				VnTarg = -(TargetAnimal->VelX * UNX + TargetAnimal->VelY * UNY);
				MagNetV = abs(Vn + VnTarg);

				/*
				if(MagNetV + abs(AngularVel) >= 10.0) {
					Damage = 100.0 * ( (OrganList[1]->Attribute1) / (OrganList[1]->Attribute1 + TargetAnimal->OrganList[1]->Attribute2) );
					if(Damage >= TargetAnimal->CurrentEnergy) Damage = TargetAnimal->CurrentEnergy;
					CurrentEnergy += Damage;
					TargetAnimal->CurrentEnergy -= Damage;

					theBrain.InputLayer[7]->Value = -Damage;

					TargetAnimal->theBrain.InputLayer[7]->Value = Damage;
					TargetAnimal->LastColTime = LastColTime;

				}*/
				//else {
					Damage = 100.0 * ( (OrganList[1]->Attribute1) / (OrganList[1]->Attribute1 + TargetAnimal->OrganList[1]->Attribute2) ) * ((MagNetV * 2.0 + (AngularVel/2.0)) / 10.0);
					if(Damage >= TargetAnimal->CurrentEnergy) Damage = TargetAnimal->CurrentEnergy;
					CurrentEnergy += Damage;
					TargetAnimal->CurrentEnergy -= Damage;

					theBrain.InputLayer[7]->Value = -Damage;

					TargetAnimal->theBrain.InputLayer[7]->Value = Damage;
					TargetAnimal->LastColTime = LastColTime;
				//}

				if(MakeBlood) {
					BloodSplatter* tempPointer = new BloodSplatter(ColX - 8, ColY - 8);
					Entity::EntityList.push_back(tempPointer);
				}
				
			}
		}
	}
	else if(BodyCol) {
		float ColX, ColY, CDX, CDY, CX, CY, CXTarg, CYTarg, Vn, Vt, VnTarg, VtTarg, UNX, UNY, UTX, UTY, VnF, VnFTarg;
		CX = X + (1/2) * (float)Surf_Entity->w;
		CY = Y + (1/2) * (float)Surf_Entity->h;
		CXTarg = TargetAnimal->X + (1/2) * (float)TargetAnimal->Surf_Entity->w;
		CYTarg = TargetAnimal->Y + (1/2) * (float)TargetAnimal->Surf_Entity->h;
		CDX = CXTarg - CX;
		CDY = CYTarg - CY;
		UNX = (CDX / sqrt(pow(CDX, 2) + pow(CDY, 2)));
		UNY = (CDY / sqrt(pow(CDX, 2) + pow(CDY, 2)));
		ColX = Radius * UNX;  //(x,y) coordinates of collision with respect to display origin
		ColY = Radius * UNY;
		Vn = (VelX * UNX) + (VelY * UNY);  //magnitude of vector component along line through centers of entities
		VnTarg = -(TargetAnimal->VelX * UNX + TargetAnimal->VelY * UNY);
		VnF = (1 / (CurrentTotalMass + TargetAnimal->CurrentTotalMass)) * (Vn * (CurrentTotalMass - TargetAnimal->CurrentTotalMass) + 2 * TargetAnimal->CurrentTotalMass * VnTarg);
		VnFTarg = (1 / (CurrentTotalMass + TargetAnimal->CurrentTotalMass)) * (VnTarg * (TargetAnimal->CurrentTotalMass - CurrentTotalMass ) + 2 * CurrentTotalMass * Vn);
		//convert back to game coord system.
		//must add conservation of angular momentum

		UTX = -UNY;
		UTY = UNX;

		float VTDir = TargetAnimal->Radius * TargetAnimal->AngularVel + UTX * TargetAnimal->VelX + UTY * TargetAnimal->VelY - Radius*AngularVel - UTX * VelX - UTY * VelY;
		VTDir = VTDir / abs(VTDir);
		
		AngularVel += ( Radius / 32.0 ) * Entity::CoFr * MaxTotalMass * sqrt ( pow( VelX - VnF * UNX, 2 ) + pow( VelY - VnF * UNY, 2 ) ) * ( 2.0 / (MaxTotalMass * pow(Radius / 32.0, 2) ) ) * -VTDir;
		TargetAnimal->AngularVel += ( TargetAnimal->Radius / 32.0 ) * Entity::CoFr * TargetAnimal->MaxTotalMass * sqrt ( pow( TargetAnimal->VelX - VnFTarg * UNX, 2 ) + pow( TargetAnimal->VelY - VnFTarg * UNY, 2 ) ) * ( 2.0 / (TargetAnimal->MaxTotalMass * pow(TargetAnimal->Radius / 32.0, 2) ) ) * VTDir;


		VelX = VnF * UNX;
		VelY = VnF * UNY;
		TargetAnimal->VelX = VnFTarg * UNX;
		TargetAnimal->VelY = VnFTarg * UNY;

		if(sqrt(pow(CDX, 2) + pow(CDY, 2)) < (9.0/10.0) * (Radius + TargetAnimal->Radius) && VnF < 0.1 && VnFTarg < 0.1) {
			if(UNX == 0 && UNY == 0) {
				UNX = -sin(Angle);
				UNY = -cos(Angle);
			}
			float SeperateDistance = (Radius + TargetAnimal->Radius - sqrt(pow(CDX, 2) + pow(CDY, 2))) * 1.25;
			X += UNX * SeperateDistance/float(2);
			Y += UNY * SeperateDistance/float(2);
			TargetAnimal->X += -UNX * SeperateDistance/float(2);
			TargetAnimal->Y += -UNY * SeperateDistance/float(2);

			/*if(VnF <= 0.1 && VnFTarg == 0.1) {
				VnF = 1;
				VnFTarg = 1;
			}

			VnF *= 4;
			VnFTarg *= 4;*/
		}
		/*else if(sqrt(pow(CDX, 2) + pow(CDY, 2)) < (8.0/10.0) * (Radius + TargetAnimal->Radius) && VnF < 1 && VnFTarg < 1) {
			if(VnF <= 0.01 && VnFTarg <= 0.01) {
				VnF = 1;
				VnFTarg = 1;
			}

			VnF *= 2;
			VnFTarg *= 2;
		}
		*/
		
		
	}

}
/*

bool Animal::PosValidEntity(Entity* TargetEntity, int NewX, int NewY) {
	//if(this != TargetEntity && TargetEntity != NULL && TargetEntity->Dead == false);
}

*/

BloodSplatter::BloodSplatter(float X, float Y): Entity(X, Y, Entity::ENTITY_TYPE_MISC, "../Debug/gfx/bloodsplatter.bmp") {
	if(Surf_Entity != NULL) Surface::Transparent(Surf_Entity, 255, 0, 255);
}

ReproduceAnimation::ReproduceAnimation(float X, float Y): Entity(X, Y, Entity::ENTITY_TYPE_MISC, "../Debug/gfx/heart.bmp") {
	if(Surf_Entity != NULL) Surface::Transparent(Surf_Entity, 255, 0, 255);
}

void ReproduceAnimation::OnLoop() {
	if( CreationTime + 2000 < SDL_GetTicks() ) Dead = true;
}

void ReproduceAnimation::OnRender(SDL_Surface* Surf_Display) {
	Surface::OnDraw(Surf_Display, Surf_Entity, (int)X - Camera::theCamera->X, (int)Y - Camera::theCamera->Y, 0, 0, Surf_Entity->w, Surf_Entity->h);
}

void BloodSplatter::OnLoop() {
	if( CreationTime + 5000 < SDL_GetTicks() ) Dead = true;
}

void BloodSplatter::OnRender(SDL_Surface* Surf_Display) {
	Surface::OnDraw(Surf_Display, Surf_Entity, (int)X - Camera::theCamera->X, (int)Y - Camera::theCamera->Y, 0, 0, Surf_Entity->w, Surf_Entity->h);
}

Corpse::Corpse(float X, float Y): Entity(X, Y, Entity::ENTITY_TYPE_MISC, "../Debug/gfx/corpse.bmp") {
	if(Surf_Entity != NULL) Surface::Transparent(Surf_Entity, 255, 0, 255);
}

void Corpse::OnLoop() {
	if( CreationTime + 8000 < SDL_GetTicks() ) Dead = true;
}

void Corpse::OnRender(SDL_Surface* Surf_Display) {
	Surface::OnDraw(Surf_Display, Surf_Entity, (int)X - Camera::theCamera->X, (int)Y - Camera::theCamera->Y, 0, 0, Surf_Entity->w, Surf_Entity->h);
}

bool Animal::OnReproduce(Animal* TargetAnimal, float PercentInputEnergy) {
	if(LastReproductionTime + 30000.0 > SDL_GetTicks()) return false; //delay of 30 sec;
	if(WorldSettings::MaxNumAnimals <= AnimalList.size()) return false; //upper limit

	float TX, TY;
	TX = X + float(2.2) * Radius * -sin(Angle + acos(-1.0));
	TY = Y + float(2.2) * Radius * -cos(Angle + acos(-1.0)); //need coordinates for top left corner of offspring's surface;

	if(TargetAnimal == NULL) {
		for(int i = 0; i < AnimalList.size(); i++) {
			if(GetAnimal(TX + float(Surf_Entity->w)/float(2), TY + float(Surf_Entity->h)/float(2))) return false;
		}
	}

	Animal* TempAnimal;
	TempAnimal = new Animal(this, &theBrain);
	TempAnimal->X = TX;
	TempAnimal->Y = TY;
	TempAnimal->Type = Entity::ENTITY_TYPE_ANIMAL;

	
	for(int i = 0; i < TempAnimal->OrganList.size(); i++) {
		TempAnimal->OrganList[i]->X += TX - X;
		TempAnimal->OrganList[i]->Y += TY - Y;
	}
	
	TempAnimal->Angle = Angle + acos(-1.0);

	TempAnimal->CurrentEnergy = PercentInputEnergy * CurrentEnergy;

	CurrentEnergy = ( 1.0 - PercentInputEnergy) * CurrentEnergy;

	Animal::AnimalList.push_back(TempAnimal);
	Entity::EntityList.push_back((Entity*) TempAnimal);
	

	Entity* TempEntity;
	TempEntity = new ReproduceAnimation(TX, TY);

	Entity::EntityList.push_back(TempEntity);

	LastReproductionTime = SDL_GetTicks();

	return true;

}


bool Animal::OnWorldr() {
	if(WorldSettings::MaxNumAnimals <= AnimalList.size()) return false; //upper limit

	float TX, TY;
	TX = X + float(2.2) * Radius * -sin(Angle + acos(-1.0));
	TY = Y + float(2.2) * Radius * -cos(Angle + acos(-1.0)); //need coordinates for top left corner of offspring's surface;

	if(Dead == true || Surf_Entity == NULL) return false;

	for(int i = 0; i < AnimalList.size(); i++) {
		if(GetAnimal(TX + float(Surf_Entity->w)/2.0, TY + float(Surf_Entity->h)/2.0)) return false;
	}
	

	Animal* TempAnimal;
	TempAnimal = new Animal(this, &theBrain);
	TempAnimal->X = TX;
	TempAnimal->Y = TY;
	TempAnimal->Type = Entity::ENTITY_TYPE_ANIMAL;

	
	for(int i = 0; i < TempAnimal->OrganList.size(); i++) {
		TempAnimal->OrganList[i]->X += TX - X;
		TempAnimal->OrganList[i]->Y += TY - Y;
	}
	
	TempAnimal->Angle = Angle + acos(-1.0);

	TempAnimal->CurrentEnergy = TempAnimal->MaxEnergy;

	CurrentEnergy = MaxEnergy;

	Animal::AnimalList.push_back(TempAnimal);
	Entity::EntityList.push_back((Entity*) TempAnimal);
	

	Entity* TempEntity;
	TempEntity = new ReproduceAnimation(TX, TY);

	Entity::EntityList.push_back(TempEntity);

	return true;
}

Animal* Animal::GetAnimal(float X, float Y) {
	float CX, CY, DX, DY;

	int UpperBound = AnimalList.size();
	for(int i = 0; i < UpperBound; i++) {
		if(AnimalList[i] != NULL && AnimalList[i]->Surf_Entity != NULL) {
			CX = AnimalList[i]->X + float(AnimalList[i]->Surf_Entity->w)/2.0;
			CY = AnimalList[i]->Y + float(AnimalList[i]->Surf_Entity->h)/2.0;

			DX = X - CX;
			DY = Y - CY;

			if( sqrt(pow(DX, 2) + pow(DY, 2)) < AnimalList[i]->Radius ) return AnimalList[i];
		}
	}

	return NULL;
}