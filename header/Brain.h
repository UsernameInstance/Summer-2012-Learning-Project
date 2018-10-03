#ifndef BRAIN_H
#define BRAIN_H

#include <SDL.h>
#include "Node.h"

class Memory {
	public:
		std::vector<float> Inputs;
		std::vector<float> Outputs;

		Memory(float InitialEnergy, std::vector<Node*> Inputs, std::vector<Node*> Outputs);

		float DelEnergy;
		float InitialEnergy;
		float FinalEnergy;
		float RInScore, RStScore, SInScore, SStScore, TotalSScore; //Raw / Scaled Input / Status Score
		int CreationTime; //in Milliseconds
		int Age;
		bool Complete;
};

class Brain {
	public:
		std::vector<Node*> InputLayer;
		std::vector<Node*> HiddenLayer;
		std::vector<Node*> OutputLayer;

		std::vector<Memory*> MemoryList; 

		/*
		struct Target {
			float Value;
			float Multiplier;
		};

		std::vector<Target*> Targets;
		*/

		Brain(int InputLayerSize, int HiddenLayerSize, int OutputLayerSize, int MaxMemListSize = (75 + rand() % 51), float MemDelay = (float)( (float)(10 + rand() % 91) / (float)100.0 ) * 2.0);
		~Brain();

		Brain(Brain* SourceBrain, bool Mutate = false);

		int MaxMemListSize;
		float MemDelay; //Delay between associating input/output to state.  In seconds.
		float LastProcessingTime; //milliseconds
		float AnimalMaxEnergy;

		void UpdateMemoryList(float CurrentEnergy);
		void UpdateWeights();

		void ProcessInputs();
};

/*
Thinking about changing how brain weight are updated.  The current vague idea, is to give each Animal Entity a set of semi-randomized "preferences".  Which are then used in place of the current: Change in Energy and Final Energy
checks which are used in the update process.

So, a quick and dirty implementation would be to change the Memory Class, by removing Delta Energy & Final Energy (and maybe some other members);  Then using the Memory InputList, as a way to check the the Animal's state at some certain time.
The InputList would be compared against the "preferences" list, and then used to score each memory, update weights, etc...

So for Example, you could have the entry in the Preferences List which corresponds to the AnimalCurrentEnergy in the InputList be equal to the Animals Maximum Energy;
This would then create a preference for input vectors with higher current energy values.

Although it ignores "cause and effect", if there is a close enough relationship between "desirable" inputs and the outputs those inputs generate, then it should work to generate targeted Animal behavior.
That is, the assumption that the actions the animal takes when it has desirable inputs are related to, or are what "caused" the attainment of that desirable InputList.

If this does not work well.  Then I can switch over the current weighted averages system to the outputs of all MemoryList entries whose Ages are greater than that entry with the "best" input vector
and less than that entry with Age greater than the best input vector's and greatest error of all others.
*/

#endif