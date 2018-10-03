#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

class Camera {
private:
	static bool InstanceFlag;
	Camera(int X = 0, int Y = 0);

public:
	static Camera* theCamera;
	static void MakeInstance(int X = 0, int Y = 0);
	static float ZoomFactor;

	int X;
	int Y;
	int TargetMode;

	int* Width;   //Display width and height.  Contained in App.
	int* Height;

	float* TargetX;  //For camera tracking an entity.  Will be center of screen.
	float* TargetY;

	

	void OnMove(int MoveX, int MoveY);

	enum {
		TARGET_MODE_NORMAL,
		TARGET_MODE_CENTER
	};

};


#endif