#ifndef CAMERA_H
#define CAMERA_H

#include "MovingObject.h"

class Camera: public MovingObject {
	public:
		Vector C;

	Camera(Vector _P, Vector _C) {
		P = _P;
		C = _C;
		V = C-P;
	}

	void Move() {
		glLoadIdentity();
		V = C-P;
		gluLookAt(
					P.x, P.y, P.z, 
					C.x, C.y, C.z,
					0, 1, 0);
	}

};

#endif