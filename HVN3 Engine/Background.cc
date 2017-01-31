#include "Background.h"

Background::Background(const char* filename) : __bitmap(filename) {}
Background::Background(Background&& other) : __bitmap(other.__bitmap) {}

unsigned int Background::Width() const {

	return __bitmap.Width();

}
unsigned int Background::Height() const {

	return __bitmap.Height();

}

const Drawing::Bitmap& Background::Bitmap() const {

	return __bitmap;

}