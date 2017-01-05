#ifndef __MOVEMENT_H
#define __MOVEMENT_H
#include "Object.h"

// Rotates an Object towards an angle at a given speed. Returns true when the angle is achieved.
bool RotateToAngle(Object& obj, float angle, float speed);

#endif