#pragma once
#include "SpriteableBase.h"

namespace hvn3 {

	// Rotates an Object towards an angle at a given speed. Returns true when the angle is achieved.
	bool RotateToAngle(SpriteableBase& obj, float angle, float speed);

}