#ifndef NODE_H
#define NODE_H

#include <SDL.h>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include "WorldSettings.h"

class Node {
	public:
		Node(int NumWeights = 0, float Bias = 0, float LearningRate = /*((2.0/3.0) + float(rand() % 67)/100.0) */ 1.25, float Momentum = (float)(5 + rand() % 16) / (float)(100));
		Node(Node* SourceNode, bool Mutate = false);

		std::vector<float> WeightList;
		std::vector<float> LastWeightList;

		std::vector<float> OriginalWeightList;  //for use in reproduction

		int NumWeights;
		float Bias;
		float LearningRate;
		float Value;
		float Momentum;

		bool ActivationFunction();
		float SigFunction(float V);
		float DSigFunction(float V); //derivative

		//Node* CopyNode();
};

#endif