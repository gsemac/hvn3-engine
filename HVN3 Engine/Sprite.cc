#include "Sprite.h"
#include "Exception.h"
#include <limits>

Sprite::Sprite() {}
Sprite::Sprite(const char* path) :
	Sprite(path, 0, 0) {}
Sprite::Sprite(const char* path, int origin_x, int origin_y) :
	__origin(origin_x, origin_y) {

	// Load the sprite from the filesystem.
	ALLEGRO_BITMAP* ptr = al_load_bitmap(path);

	// If we failed to access the file, throw an error.
	if (!ptr)
		throw IO::IOException("Failed to access file '" + std::string(path) + "'.");

	// Push the bitmap into the frame collection.
	__frames.push_back(ptr);

	// Set width and height members.
	__width = al_get_bitmap_width(ptr);
	__height = al_get_bitmap_height(ptr);

	// (These members are only relevant for sprite sheets, so set them to null.)
	__sheet = nullptr;
	__strip_length = 0;

}
Sprite::Sprite(const char* path, int origin_x, int origin_y, const Color& alpha_color)
	: Sprite(path, origin_x, origin_y) {

	if (__frames[0])
		al_convert_mask_to_alpha(__frames[0], alpha_color.AlPtr());

}
Sprite::Sprite(const std::string& path) :
	Sprite(path.c_str()) {}
Sprite::Sprite(const std::string& path, int origin_x, int origin_y) :
	Sprite(path.c_str(), origin_x, origin_y) {}
Sprite::Sprite(const std::string& path, int origin_x, int origin_y, const Color& alpha_color)
	: Sprite(path.c_str(), origin_x, origin_y, alpha_color) {}
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

Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height) {

	return FromSpriteSheet(path, frame_width, frame_height, 0, 0);

}
Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y) {

	// Load the sprite sheet from the filesystem.
	ALLEGRO_BITMAP* sheet = al_load_bitmap(path);

	// If we failed to access the file, throw an error.
	if (!sheet)
		throw IO::IOException("Failed to access file '" + std::string(path) + "'.");

	// Get the dimensions of the sheet, and determine the number of rows/columns.
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

	//return FromSpriteSheet(path, frame_width, frame_height, 0, 0, 0, 0, INT_MAX, origin_x, origin_y);

}
Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color) {

	Sprite sprite = FromSpriteSheet(path, frame_width, frame_height, origin_x, origin_y);

	for (size_t i = 0; i < sprite.__frames.size(); ++i)
		al_convert_mask_to_alpha(sprite.__frames[i], alpha_color.AlPtr());

	return sprite;

}
Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y) {

	throw NotImplementedException();

}
Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color) {

	throw NotImplementedException();

}
Sprite Sprite::FromSpriteSheet(const std::string& path, int frame_width, int frame_height) {

	return FromSpriteSheet(path.c_str(), frame_width, frame_height, 0, 0);

}
Sprite Sprite::FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y) {

	return FromSpriteSheet(path.c_str(), frame_width, frame_height, origin_x, origin_y);

}
Sprite Sprite::FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color) {

	return FromSpriteSheet(path.c_str(), frame_width, frame_height, origin_x, origin_y, alpha_color);

}
Sprite Sprite::FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y) {

	return FromSpriteSheet(path.c_str(), frame_width, frame_height, frame_x_offset, frame_y_offset, frame_x_separation, frame_y_separation, frame_number, origin_x, origin_y);

}
Sprite Sprite::FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color) {

	return FromSpriteSheet(path.c_str(), frame_width, frame_height, frame_x_offset, frame_y_offset, frame_x_separation, frame_y_separation, frame_number, origin_x, origin_y, alpha_color);

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
