#include "IPositionable.h"

IPositionable::IPositionable() : IPositionable(0.0f, 0.0f) {}
IPositionable::IPositionable(float x, float y) {

	this->X = x;
	this->Y = y;

}

//void IPositionable::Move(float x, float y) {
//
//	X = x;
//	Y = y;
//
//}