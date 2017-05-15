#include "IPositionable.h"

IPositionable::IPositionable() : IPositionable(0.0f, 0.0f) {}
IPositionable::IPositionable(float x, float y) {

	this->__x = x;
	this->__y = y;

}
float IPositionable::X() const {

	return __x;

}
float IPositionable::Y() const {

	return __y;

}
void IPositionable::SetX(float x) {

	__x = x;

}
void IPositionable::SetY(float y) {

	__y = y;

}
void IPositionable::SetXY(float x, float y) {

	__x = x;
	__y = y;

}
void IPositionable::TranslateX(float x_offset) {

	SetX(X() + x_offset);

}
void IPositionable::TranslateY(float y_offset) {

	SetY(Y() + y_offset);

}
void IPositionable::Translate(float x_offset, float y_offset) {

	TranslateX(x_offset);
	TranslateY(y_offset);

}

//void IPositionable::Move(float x, float y) {
//
//	X = x;
//	Y = y;
//
//}