#pragma once
#include "ISpriteable.h"

namespace hvn3 {

	// Rotates an Object towards an angle at a given speed. Returns true when the angle is achieved.
	bool RotateToAngle(ISpriteable& obj, float angle, float speed);

}