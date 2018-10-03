#include "Camera.h"

bool Camera::InstanceFlag = false;
Camera* Camera::theCamera = NULL;
float Camera::ZoomFactor = 1;

void Camera::MakeInstance(int X, int Y) {
	if(!InstanceFlag) {
		theCamera = new Camera(X, Y);
		InstanceFlag = true;
	}
}

Camera::Camera(int X, int Y) {
	this->X = X;
	this->Y = Y;

	TargetMode = TARGET_MODE_NORMAL;

	TargetX = NULL;
	TargetY = NULL;
}

void Camera::OnMove(int MoveX, int MoveY) {
	X += MoveX;
	Y += MoveY;
}