#include "Sprite.h"

Sprite::Sprite() {}
Sprite::Sprite(Sprite&& other) {

	__frames = other.__frames;
	__sheet = other.__sheet;
	__width = other.__width;
	__height = other.__height;
	__origin = other.__origin;
	__strip_length = other.__strip_length;

	other.__frames.clear();
	other.__sheet = nullptr;
	other.__width = 0;
	other.__height = 0;
	other.__origin = Point(0, 0);
	other.__strip_length = 0;

}
Sprite::~Sprite() {

	// Dispose of all frames.
	for (size_t i = 0; i < __frames.size(); ++i)
		al_destroy_bitmap(__frames[i]);
	__frames.clear();

	// Dispose of the sprite sheet, if applicable.
	if (__sheet) {
		al_destroy_bitmap(__sheet);
		__sheet = nullptr;
	}

}
Sprite::Sprite(const char* filename, int origin_x, int origin_y) : __origin(origin_x, origin_y) {

	__sheet = nullptr;
	__frames.push_back(al_load_bitmap(filename));
	__width = al_get_bitmap_width(__frames[0]);
	__height = al_get_bitmap_height(__frames[0]);
	__strip_length = 0;

}

Sprite Sprite::FromSpriteSheet(const char* filename, int frame_width, int frame_height, int origin_x, int origin_y) {

	// Load the sprite sheet and determine relevant parameters.
	ALLEGRO_BITMAP* sheet = al_load_bitmap(filename);
	int sheet_width = al_get_bitmap_width(sheet);
	int sheet_height = al_get_bitmap_height(sheet);
	int columns = sheet_width / frame_width;
	int rows = sheet_height / frame_height;

	// Initialize the new sprite. Note that the sheet must be maintained, because sub-bitmaps share its memory.
	Sprite spr;
	spr.__sheet = sheet;
	spr.__origin = Point(origin_x, origin_y);
	spr.__width = frame_width;
	spr.__height = frame_height;
	spr.__strip_length = columns;

	// Create sub-sprites from the sheet.
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			spr.Add(al_create_sub_bitmap(sheet, frame_width * j, frame_height * i, frame_width, frame_height));

	// Return the new sprite.
	return spr;

}

unsigned int Sprite::Width() const {

	return __width;

}
unsigned int Sprite::Height() const {

	return __height;

}
unsigned int Sprite::StripLength() const {

	return __strip_length > 0 ? __strip_length : (int)__frames.size();

}
unsigned int Sprite::Length() const {

	return (unsigned int)__frames.size();

}
Point Sprite::Origin() const {

	return __origin;

}
void Sprite::SetOrigin(float origin_x, float origin_y) {

	__origin.X = origin_x;
	__origin.Y = origin_y;

}

ALLEGRO_BITMAP* Sprite::AlPtr() const {

	return AlPtr(0);

}
ALLEGRO_BITMAP* Sprite::AlPtr(int frame) const {

	frame = (__frames.size() == 0) ? 0 : frame % __frames.size();
	return __frames[frame];

}

void Sprite::Add(ALLEGRO_BITMAP* bitmap_frame) {

	__frames.push_back(bitmap_frame);

}
void Sprite::Add(const char* filename) {

	Add(al_load_bitmap(filename));

}

ALLEGRO_BITMAP* Sprite::operator[] (const int frame) const {

	return AlPtr(frame);

}
