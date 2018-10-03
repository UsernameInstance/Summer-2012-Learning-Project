#include "Node.h"

Node::Node(int NumWeights, float Bias, float LearningRate, float Momentum) {
	this->NumWeights = NumWeights;
	this->Bias = Bias;
	this->LearningRate = LearningRate;
	Value = 0;

	int Exponent;

	for(int i = 0; i < NumWeights; i++) { 
		//if(!(rand() % 6)) Exponent = 1;
		//else Exponent = 2;

		WeightList.push_back( (float)( 1 + rand() % 3 ) * pow((float) -1, rand()%2) );
		OriginalWeightList.push_back(WeightList[WeightList.back()]);
		LastWeightList.push_back(0);
	}
}

Node::Node(Node* SourceNode, bool Mutate) {

	NumWeights = SourceNode->NumWeights;
	Bias = SourceNode->Bias;
	LearningRate = SourceNode->LearningRate;
	Value = 0;

	float SSize = 1.0/WorldSettings::MutationRate;
	
	if(Mutate) {
		float MutationM;
		MutationM = float(75 + rand() % 50)/100.0;

		for(int i = 0; i < SourceNode->WeightList.size(); i++) { 
			if( !(rand() % (int)SSize ) ) { WeightList.push_back( SourceNode->WeightList[i] * MutationM ); }
			else WeightList.push_back( SourceNode->WeightList[i] );

			if( !(rand() % (int)SSize ) ) {WeightList.back() *= -1.0; }
			
			OriginalWeightList.push_back(WeightList.back());
			LastWeightList.push_back(0);
		}
	}
	else {
		for(int i = 0; i < SourceNode->WeightList.size(); i++) { 

			WeightList.push_back( SourceNode->OriginalWeightList[i] );
			OriginalWeightList.push_back(WeightList.back());
			LastWeightList.push_back(0);
		}
	}
}

bool Node::ActivationFunction() {
	if( Value - Bias > 0 ) return true;
	
	return false;
}

float Node::SigFunction(float V) {
	float e = 2.71828;
	//float ReturnValue = ( 2 / ( 1 + pow(e, -V) ) ) - 1;
	return float(2) * ( float(1) / ( float(1) + pow(e, -V) ) ) - float(1);
}

float Node::DSigFunction(float V) {
	float e = 2.71828;

	return float(2) * pow(e, -V) * pow( ( float(1) + pow(e, -V) ), -2);
}
/*
Node* Node::CopyNode() {
	Node* tempNode = new Node();
	tempNode->NumWeights = NumWeights;
	tempNode->Bias = Bias;
	tempNode->LearningRate = LearningRate;
	tempNode->Value = Value;

}
*/