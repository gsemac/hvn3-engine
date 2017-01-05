#include "ISizeable.h"

ISizeable::ISizeable(float width, float height) {

	__width = width;
	__height = height;

}

float ISizeable::Width() const {

	return __width;

}
float ISizeable::Height() const {

	return __height;

}
void ISizeable::SetWidth(float value) {

	Resize(value, __height);

}
void ISizeable::SetHeight(float value) {

	Resize(__width, value);

}
Size ISizeable::Dimensions() const {

	return Size(__width, __height);

}
void ISizeable::Resize(float width, float height) {

	__width = width;
	__height = height;

}