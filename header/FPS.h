#ifndef FPS_H
#define FPS_H

#include <SDL.h>
#include "WorldSettings.h"

class FPS {
	private:
		static bool InstanceFlag;
		FPS();

	public:
		static FPS* FPSControl;  //The static App pointer
		static void MakeInstance(); //implement a singleton
		~FPS();

		int OldTime;
		int LastTime;

		float SpeedFactor;

		int NumFrames;
		int Frames;

		void OnLoop();

		int GetFPS();

		float GetSpeedFactor();
};

#endif