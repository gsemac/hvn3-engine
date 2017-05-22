#pragma once
#include "Object.h"

namespace hvn3 {

	// Rotates an Object towards an angle at a given speed. Returns true when the angle is achieved.
	bool RotateToAngle(Object& obj, float angle, float speed);

}