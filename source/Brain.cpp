#include "Brain.h"

Memory::Memory(float InitialEnergy, std::vector<Node*> Inputs, std::vector<Node*> Outputs) {
	this->InitialEnergy = InitialEnergy;
	FinalEnergy = 0;
	TotalSScore = 0;
	Complete = false;
	CreationTime = SDL_GetTicks();
	Age = 0;

	for(int i = 0; i < Inputs.size(); i++) {
		this->Inputs.push_back(Inputs[i]->Value);
	}

	for(int i = 0; i < Outputs.size(); i++) {
		this->Outputs.push_back(Outputs[i]->Value);
	}

	RInScore = 0; RStScore = 0; SInScore = 0; SStScore = 0;
}

Brain::Brain(int InputLayerSize, int HiddenLayerSize, int OutputLayerSize, int MaxMemListSize, float MemDelay) {
	if(InputLayerSize < 0) InputLayerSize = 0;
	if(HiddenLayerSize < 0) HiddenLayerSize = 0;
	if(OutputLayerSize < 0) OutputLayerSize = 0;
	if(MaxMemListSize < 0) MaxMemListSize = 0;

	this->MaxMemListSize = MaxMemListSize;
	this->MemDelay = MemDelay;
	LastProcessingTime = 0;

	Node* tempNode = NULL; 

	for(int i = 0; i < InputLayerSize; i++) {
		tempNode = new Node(HiddenLayerSize);

		InputLayer.push_back(tempNode);
	
	}

	for(int i = 0; i < HiddenLayerSize; i++) {
		tempNode = new Node(OutputLayerSize);

		HiddenLayer.push_back(tempNode);
	}

	for(int i = 0; i < OutputLayerSize; i++) {
		tempNode = new Node(0, 1);

		OutputLayer.push_back(tempNode);
	}

}

Brain::Brain(Brain* SourceBrain, bool Mutate) {
	float SSize = 1.0/WorldSettings::MutationRate;
	
	if(Mutate) {
		float MutationM;
		MutationM = float(75 + rand() % 50)/100.0;

		if( !(rand() % (int)SSize ) ) {
			if(SourceBrain->MaxMemListSize * MutationM > 200.0) this->MaxMemListSize = 200.0; //200 current max
			else this->MaxMemListSize = SourceBrain->MaxMemListSize * MutationM;
		}

		if( !(rand() % (int)SSize ) ) {
			if(SourceBrain->MemDelay * MutationM > 10) this->MemDelay = 10;
			else this->MemDelay = SourceBrain->MemDelay;
			if(MemDelay < 0) MemDelay = 0;
		}

		LastProcessingTime = 0;

		Node* tempNode = NULL; 
		int HiddenLayerSize;

		if( !(rand() % (int)SSize ) ) {
			if(int(SourceBrain->HiddenLayer.size() * MutationM) > 40) HiddenLayerSize = 40;
			else if( int(SourceBrain->HiddenLayer.size() * MutationM) < 1 ) HiddenLayerSize = 2;
			else HiddenLayerSize = int(float(SourceBrain->InputLayer.size()) * MutationM);
		}

		for(int i = 0; i < SourceBrain->InputLayer.size(); i++) {
			tempNode = new Node(SourceBrain->InputLayer[i], true);

			if(HiddenLayerSize != SourceBrain->HiddenLayer.size()) {
				if(HiddenLayerSize > SourceBrain->HiddenLayer.size()) {
					int Diff = HiddenLayerSize - SourceBrain->HiddenLayer.size();
					
					tempNode->NumWeights = HiddenLayerSize;

					for(int j = 0; j < Diff; j++) {
						tempNode->WeightList.push_back( (float)( 1 ) * pow((float) -1, rand()%2) );
						tempNode->OriginalWeightList.push_back(tempNode->WeightList.back());
						tempNode->LastWeightList.push_back(0);
					}
				}
				else if(HiddenLayerSize < SourceBrain->HiddenLayer.size()) {
					int Diff = SourceBrain->HiddenLayer.size() - HiddenLayerSize;

					tempNode->NumWeights = HiddenLayerSize;

					for(int j = 0; j < Diff; j++) {
						tempNode->WeightList.pop_back();
						tempNode->OriginalWeightList.pop_back();
						tempNode->LastWeightList.pop_back();
					}

				}
			}

			if( !(rand() % (int)SSize ) ) { tempNode->Bias *= MutationM; }
			if( !(rand() % (int)SSize ) ) { tempNode->LearningRate *= MutationM; }
			if( !(rand() % (int)SSize ) ) { tempNode->Momentum *= MutationM; }

			InputLayer.push_back(tempNode);
		}

		for(int i = 0; i < SourceBrain->HiddenLayer.size(); i++) {
			tempNode = new Node(SourceBrain->HiddenLayer[i], true);

			if( !(rand() % (int)SSize ) ) { tempNode->Bias *= MutationM; }
			if( !(rand() % (int)SSize ) ) { tempNode->LearningRate *= MutationM; }
			if( !(rand() % (int)SSize ) ) { tempNode->Momentum *= MutationM; }

			HiddenLayer.push_back(tempNode);
		}

		
		if(HiddenLayerSize != SourceBrain->HiddenLayer.size()) {
			if(HiddenLayerSize > SourceBrain->HiddenLayer.size()) {
				int Diff = HiddenLayerSize - SourceBrain->HiddenLayer.size();

				for(int j = 0; j < Diff; j++) {
					tempNode = new Node(SourceBrain->OutputLayer.size(), true);

					HiddenLayer.push_back(tempNode);
				}
			}
			else if(HiddenLayerSize < SourceBrain->HiddenLayer.size()) {
				int Diff = SourceBrain->HiddenLayer.size() - HiddenLayerSize;

				tempNode->NumWeights = HiddenLayerSize;

				for(int j = 0; j < Diff; j++) {
					delete HiddenLayer[HiddenLayer.size() - 1];

					HiddenLayer.erase(HiddenLayer.end() - 1);
				}

			}
		}

		for(int i = 0; i < SourceBrain->OutputLayer.size(); i++) {
			tempNode = new Node(SourceBrain->OutputLayer[i], true);

			if( !(rand() % (int)SSize ) ) { tempNode->Bias *= MutationM; }
			if( !(rand() % (int)SSize ) ) { tempNode->LearningRate *= MutationM; }
			if( !(rand() % (int)SSize ) ) { tempNode->Momentum *= MutationM; }

			OutputLayer.push_back(tempNode);
		}
	}
	else { 
		this->MaxMemListSize = SourceBrain->MaxMemListSize;
		this->MemDelay = SourceBrain->MemDelay;
		LastProcessingTime = 0;

		Node* tempNode = NULL; 

		for(int i = 0; i < SourceBrain->InputLayer.size(); i++) {
			tempNode = new Node(SourceBrain->InputLayer[i]);

			InputLayer.push_back(tempNode);
		}

		for(int i = 0; i < SourceBrain->HiddenLayer.size(); i++) {
			tempNode = new Node(SourceBrain->HiddenLayer[i]);

			HiddenLayer.push_back(tempNode);
		}

		for(int i = 0; i < SourceBrain->OutputLayer.size(); i++) {
			tempNode = new Node(SourceBrain->OutputLayer[i]);

			OutputLayer.push_back(tempNode);
		}
	}
}

Brain::~Brain() {
	for(int i = 0; i < InputLayer.size(); i++) {
		delete InputLayer[i];
	}

	for(int i = 0; i < HiddenLayer.size(); i++) {
		delete HiddenLayer[i];
	}

	for(int i = 0; i < OutputLayer.size(); i++) {
		delete OutputLayer[i];
	}

	for(int i = 0; i < MemoryList.size(); i++) {
		delete MemoryList[i];
	}

	InputLayer.clear();
	HiddenLayer.clear();
	OutputLayer.clear();
	MemoryList.clear();
}


void Brain::ProcessInputs() {
	
	for(int j = 0; j < HiddenLayer.size(); j++) {
		HiddenLayer[j]->Value = 0;
		for(int k = 0; k < InputLayer.size(); k++) {
			if(InputLayer[k]->ActivationFunction()) HiddenLayer[j]->Value += InputLayer[k]->WeightList[j] * InputLayer[k]->SigFunction(InputLayer[k]->Value);
		}
	}

	for(int i = 0; i < OutputLayer.size(); i++) {
		OutputLayer[i]->Value = 0;
		for(int j = 0; j < HiddenLayer.size(); j++) {
			if(HiddenLayer[j]->ActivationFunction()) OutputLayer[i]->Value += HiddenLayer[j]->WeightList[i] * HiddenLayer[j]->SigFunction(HiddenLayer[j]->Value);
		}
	}

	
}

void Brain::UpdateMemoryList(float CurrentEnergy) {
	Memory* tempMemory = NULL;
	float ErrorTolerance = 0.1;
	float RelError = 0, AbsError = 0, AbsSum = 0;
	// When MemoryList is full, when updating, the last memory in the list replaces some memory higher up on the list if it has superior results and if the relative error of the input terms is within some tolerance.  That replaced memory is deleted and the list is resorted in order of memory age.
	//Newest is first, oldest is last.

	if(MemoryList.size() < 1) {
		tempMemory = new Memory(CurrentEnergy, InputLayer, OutputLayer);
		MemoryList.push_back(tempMemory);
	}
	else {
		if(MemoryList.back()->Complete) {
			tempMemory = new Memory(CurrentEnergy, InputLayer, OutputLayer);
			MemoryList.push_back(tempMemory);
		}
		else {
			MemoryList.back()->FinalEnergy = CurrentEnergy;
			MemoryList.back()->DelEnergy = (CurrentEnergy - MemoryList.back()->InitialEnergy) / ((float)(SDL_GetTicks() - MemoryList.back()->CreationTime));
			MemoryList.back()->Complete = true;
		}

		/*
		if(MemoryList.size() >= MaxMemListSize) {
			delete MemoryList[0];
			MemoryList.erase( MemoryList.begin() );
		}
		*/
	}

	if(MemoryList.size() >= MaxMemListSize) {
		int BI = -1;
		int LBI = -1;
		float LowestRelativeError = -1;

		for(int i = 1; i < MemoryList.size() - 2; i ++) {
			AbsError = 0;

			for(int j = 1; j < MemoryList[MemoryList.size()]->Inputs.size(); j++) {
				AbsError += pow(MemoryList[0]->Inputs[j] - MemoryList[i]->Inputs[j], 2);
				if(i == 1) AbsSum += pow(MemoryList[0]->Inputs[j], 2);

			}

			AbsError = sqrt(AbsError);
			AbsSum = sqrt(AbsSum);

			RelError = AbsError / AbsSum;
			if(i == 1) LowestRelativeError = RelError;

			if( ( RelError < 0.1 && MemoryList[i]->FinalEnergy <= MemoryList[0]->FinalEnergy ) ) {
				if( MemoryList[i]->FinalEnergy == MemoryList[0]->FinalEnergy && MemoryList[i]->DelEnergy < MemoryList[0]->DelEnergy ) {
					if(BI == -1) BI = i;
					else if(MemoryList[i]->FinalEnergy < MemoryList[BI]->FinalEnergy) BI = i;
				}
				else if ( MemoryList[i]->FinalEnergy < MemoryList[0]->FinalEnergy ) {
					if(BI == -1) BI = i;
					else if(MemoryList[i]->FinalEnergy < MemoryList[BI]->FinalEnergy) BI = i;
				}
			}
			if( RelError < LowestRelativeError) {
				LowestRelativeError = RelError;
				LBI = i;
			}

		}
			
		if(BI == -1 && LowestRelativeError >= 0.9 && ( MemoryList[0]->FinalEnergy >= AnimalMaxEnergy || MemoryList[0]->DelEnergy > 0 ) ) BI = LBI; //if every one in the list has error >= 0.9 relative to the last element.  The last element is fairly unique and is kept.

		if(BI > 0) {
			delete MemoryList[BI];
			MemoryList.erase(MemoryList.begin() + BI); // Position BI

			MemoryList.insert(MemoryList.end() - 2, MemoryList[0]);
			MemoryList.erase(MemoryList.begin());
		}
		else {
			delete MemoryList[0];
			MemoryList.erase(MemoryList.begin());
		}
	}

}
void Brain::UpdateWeights() {
	if( MemoryList.size() < 2) return;
	int BI = 0; //BestIndex
	float SumRInScore = 0, SumRStScore = 0, BFE = MemoryList[0]->FinalEnergy, BDE = MemoryList[0]->DelEnergy, BestTotalScore = 0, LFE = 0, LDE = 0;
	float NextWeight;
	std::vector<float> TargetOutputs;
	int BA = -1, WA = -1; //Age of best memory, age of worst memory less than best age;

	for(int i = 0; i < MemoryList.size() - 2; i++) {
		MemoryList[i]->Age += SDL_GetTicks() - MemoryList[i]->CreationTime;

		if(i == 0) {
			BFE = MemoryList[i]->FinalEnergy;
			BDE = MemoryList[i]->DelEnergy;
			BI = 1;
		}
		else if(MemoryList[i]->FinalEnergy + MemoryList[i]->DelEnergy  > BFE + BDE) {
			BFE = MemoryList[i]->FinalEnergy;
			BDE = MemoryList[i]->DelEnergy;
			BI = i;
		}
	}

	BA = MemoryList[BI]->Age;
	LFE = BFE;
	LDE = BDE;
	int LI = -1;

	for(int i = 0; i < MemoryList.size() - 1; i++) {
		if(MemoryList[i]->FinalEnergy + MemoryList[i]->DelEnergy < LFE + LFE && MemoryList[i]->Age < BA) {
			LFE = MemoryList[i]->FinalEnergy;
			LDE = MemoryList[i]->DelEnergy;
			LI = i;
		}
	}

	if(LI >= 0) WA = MemoryList[LI]->Age;
	else LI = BI;

	for(int i = 0; i < MemoryList.size() - 1; i++) {
		MemoryList[i]->RInScore = 0;
		MemoryList[i]->RStScore = 0;

		if(MemoryList[i]->Age <= BA && MemoryList[i]->Age >= WA) {
			for(int j = 0; j < InputLayer.size(); j++) {  
				if(j != 12) MemoryList[i]->RInScore += pow( MemoryList[i]->Inputs[j] - InputLayer[j]->Value , 2 );
			}

			MemoryList[i]->RInScore = sqrt(MemoryList[i]->RInScore);  //these are actually error measurements
			if(AnimalMaxEnergy > BFE) MemoryList[i]->RStScore = sqrt( pow(BFE - MemoryList[i]->FinalEnergy, 2 ) + pow(BDE - MemoryList[i]->DelEnergy, 2 ) );
			else MemoryList[i]->RStScore = sqrt( pow(BDE - MemoryList[i]->DelEnergy, 2 ) );

			SumRInScore += MemoryList[i]->RInScore;
			SumRStScore += MemoryList[i]->RStScore;
		}
	}

	for(int i = 0; i < OutputLayer.size(); i++) {
		TargetOutputs.push_back(0);

		for(int j = 0; j < MemoryList.size() - 1; j++) {

			if(MemoryList[j]->Age <= BA && MemoryList[j]->Age >= WA) {
				if(SumRInScore != 0 && SumRStScore != 0) MemoryList[j]->TotalSScore = ( ( float(1) - (MemoryList[j]->RInScore) / (SumRInScore) ) + ( (float(1) - (MemoryList[j]->RStScore) / (SumRStScore) ) ) );
				else if(SumRInScore != 0 && SumRStScore == 0) MemoryList[j]->TotalSScore = ( float(1) - (MemoryList[j]->RInScore) / (SumRInScore) );
				else if(SumRInScore == 0 && SumRStScore != 0) MemoryList[j]->TotalSScore = ( (float(1) - (MemoryList[j]->RStScore) / (SumRStScore) ) );
				else MemoryList[j]->TotalSScore = 1;

				if(BA - WA > 0) MemoryList[j]->TotalSScore *= ( 1.0 - ( float(MemoryList[j]->Age) / float(BA - WA) ) );

				if(BFE < AnimalMaxEnergy && BDE < 0) {
					TargetOutputs[i] -= MemoryList[j]->Outputs[i] * MemoryList[j]->TotalSScore; 
				}
				else TargetOutputs[i] += MemoryList[j]->Outputs[i] * MemoryList[j]->TotalSScore; 
			}

		}
	}

	/*
	if(OutputLayer.size() == 3) TargetOutputs[2] += (32.0/7.0) * pow((InputLayer[12]->Value / AnimalMaxEnergy), 2) - 18.0/7.0; //(-16.0/7.0) * log(1.0/3.0) * pow((InputLayer[12]->Value / AnimalMaxEnergy), 2) + (16.0/7.0) * log(1.0/3.0);
	if(OutputLayer.size() == 4) TargetOutputs[3] += (32.0/7.0) * pow((InputLayer[12]->Value / AnimalMaxEnergy), 2) - 18.0/7.0;
	*/
	
	/*
	for(int i = 1; i < MemoryList.size(); i++) {
		if(MemoryList[i]->FinalEnergy > BFE) BFE = MemoryList[i]->FinalEnergy;
		if(MemoryList[i]->DelEnergy > BDE) BDE = MemoryList[i]->DelEnergy;
	}

	
	for(int i = 1; i < MemoryList.size(); i++) {
		MemoryList[i]->RInScore = 0;

		for(int j = 0; j < InputLayer.size() - 2; j++) {  //subtract 1 from InputLayer.size() so that CurrentEnergyNodes is not included in inputs check.
			MemoryList[i]->RInScore += pow( MemoryList[i]->Inputs[j] - InputLayer[j]->Value , 2 );
		}

		MemoryList[i]->RInScore = sqrt(MemoryList[i]->RInScore);  //these are actually error measurements
		MemoryList[i]->RStScore = sqrt( pow( MemoryList[i]->FinalEnergy - BFE, 2 ) + pow( MemoryList[i]->DelEnergy - BDE, 2 ) );

		SumRInScore += MemoryList[i]->RInScore;
		SumRStScore += MemoryList[i]->RStScore;
	}

	if( SumRInScore == 0 ) SumRInScore += 1;
	if( SumRStScore == 0 ) SumRStScore += 1;
	
	for(int i = 1; i < MemoryList.size(); i++) {
		//MemoryList[i]->SInScore = ( 1 - (MemoryList[i]->RInScore / SumRInScore) );
		MemoryList[i]->SStScore = (1.5) * ( 1.0 - (MemoryList[i]->RInScore / SumRInScore) );
		MemoryList[i]->TotalSScore = MemoryList[i]->SInScore + MemoryList[i]->SStScore;

		if(BestTotalScore < MemoryList[i]->TotalSScore) {
			BestTotalScore = MemoryList[i]->TotalSScore;
			BI = i;
		}
	}
	*/
	
	
	bool Randomize = false;
	float RandomM = 1;
	
	/*
	if((BFE < AnimalMaxEnergy ) && MemoryList.size() == MaxMemListSize) if(!(rand() % 100)) Randomize = true;
	else if (BFE < AnimalMaxEnergy && MemoryList.size() >= MaxMemListSize/2) if(!(rand() % 250)) Randomize = true;
	else if (BFE < AnimalMaxEnergy && MemoryList.size() < MaxMemListSize/2) if(!(rand() % 500)) Randomize = true;
	*/
	/*
	if(BDE < 0 && BFE < AnimalMaxEnergy && MemoryList.size() == MaxMemListSize) if(!(rand() % 100)) Randomize = true;
	else if (BDE < 0 && BFE < AnimalMaxEnergy && MemoryList.size() >= MaxMemListSize/2) if(!(rand() % 250)) Randomize = true;
	else if (BDE < 0 && BFE < AnimalMaxEnergy && MemoryList.size() < MaxMemListSize/2) if(!(rand() % 500)) Randomize = true;
	*/

	float Error = 0, Sum = 0;
	//input to hidden
	for(int i = 0; i < InputLayer.size(); i++) {
		for(int j = 0; j < InputLayer[i]->WeightList.size(); j++) {
			for(int u = 0; u < InputLayer.size(); u++) { Sum += InputLayer[u]->WeightList[j] * InputLayer[u]->Value; }

			Error = 0;
			for(int k = 0; k < OutputLayer.size(); k++) {
				if(Randomize) RandomM = pow(-1.0, rand() % 2) * ( (float)(5 + rand() % 11) / (float)10 ) * 10;
				Error += InputLayer[i]->WeightList[k] * (TargetOutputs[k] * RandomM - OutputLayer[k]->Value);
			}

			Error = -InputLayer[i]->Value * InputLayer[i]->DSigFunction(Sum) * Error;

			NextWeight = InputLayer[i]->WeightList[j] + -InputLayer[i]->LearningRate * Error + InputLayer[i]->Momentum *(InputLayer[i]->LastWeightList[j]);
			InputLayer[i]->LastWeightList[j] = InputLayer[i]->WeightList[j];
			InputLayer[i]->WeightList[j] = NextWeight;
		}
	}
	//hidden to output
	Error = 0;
	Sum = 0;
	for(int i = 0; i < HiddenLayer.size(); i++) {
		for(int u = 0; u < InputLayer.size(); u++) { Sum += InputLayer[u]->WeightList[i] * InputLayer[u]->Value; }
		
		for(int j = 0; j < HiddenLayer[i]->WeightList.size(); j++) {
			if(Randomize) RandomM = pow(-1.0, rand() % 2) * ( (float)(1 + rand() % 11) / (float)10 );
			Error = -HiddenLayer[i]->SigFunction(Sum) * (TargetOutputs[j] * RandomM - OutputLayer[j]->Value);

			NextWeight = HiddenLayer[i]->WeightList[j] + -HiddenLayer[i]->LearningRate * Error + HiddenLayer[i]->Momentum * (HiddenLayer[i]->LastWeightList[j]);
			HiddenLayer[i]->LastWeightList[j] = HiddenLayer[i]->WeightList[j];
			HiddenLayer[i]->WeightList[j] = NextWeight;
		}
	}

}
