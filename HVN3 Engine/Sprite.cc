#include "Sprite.h"
#include "io/Directory.h"
#include "Exception.h"
#include "Framework.h"
#include <limits>
#include <cmath>

Sprite::Sprite() {}
Sprite::Sprite(const char* path) :
	Sprite(path, 0, 0) {}
Sprite::Sprite(const char* path, int origin_x, int origin_y) :
	__origin(origin_x, origin_y) {

	// Load the image from the filesystem.
	Drawing::Bitmap bmp(path);

	// If we failed to access the file, throw an error.
	if (!bmp)
		throw IO::IOException("Failed to access file '" + std::string(path) + "'.");

	// We are not using a sprite sheet, so set these values accordingly.
	__using_sprite_sheet = false;
	__strip_length = 0;

	// Add the image to the subimage collection.
	Add(bmp, true);

}
Sprite::Sprite(const char* path, int origin_x, int origin_y, const Color& alpha_color)
	: Sprite(path, origin_x, origin_y) {

	// If the first frame was added successfully, remove the background color.
	if (__frames[__using_sprite_sheet])
		__frames[__using_sprite_sheet].ConvertMaskToAlpha(alpha_color);

}
Sprite::Sprite(const std::string& path) :
	Sprite(path.c_str()) {}
Sprite::Sprite(const std::string& path, int origin_x, int origin_y) :
	Sprite(path.c_str(), origin_x, origin_y) {}
Sprite::Sprite(const std::string& path, int origin_x, int origin_y, const Color& alpha_color)
	: Sprite(path.c_str(), origin_x, origin_y, alpha_color) {}
Sprite::Sprite(Sprite&& other) {

	// Move all frames to the other Sprite.
	for (size_t i = 0; i < other.__frames.size(); ++i)
		__frames.push_back(std::move(other.__frames[i]));

	__origin = other.__origin;
	__strip_length = other.__strip_length;
	__using_sprite_sheet = other.__using_sprite_sheet;

	other.__origin = Point(0, 0);
	other.__strip_length = 0;
	other.__using_sprite_sheet = false;

}
Sprite::~Sprite() {

	// If the framework has been shut down before freeing resources, do nothing (or else we'd get an error), but alert the user.
	if (!Framework::Initialized()) {
		std::cerr << "Warning: Framework was shutdown before this resource could be freed. Free all resources before shutting down the framework.\n";
		return;
	}

	// Dispose of all frames.
	__frames.clear();

}

Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height) {

	return FromSpriteSheet(path, frame_width, frame_height, 0, 0);

}
Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y) {

	// Load the sprite sheet from the filesystem.
	Drawing::Bitmap sheet(path);

	// If we failed to access the file, throw an error.
	if (!sheet)
		throw IO::IOException("Failed to access file '" + std::string(path) + "'.");

	// Get the dimensions of the sheet, and determine the number of rows/columns.
	int sheet_width = sheet.Width();
	int sheet_height = sheet.Height();
	int columns = sheet_width / frame_width;
	int rows = sheet_height / frame_height;

	// Initialize the new Sprite. Note that the sheet must be maintained, because sub-bitmaps share its memory.
	// The sheet will be maintained as the first of the subimages; calls to the first subimage will return the second, and so on.
	Sprite spr;
	spr.__origin = Point(origin_x, origin_y);
	spr.__strip_length = columns;
	spr.__using_sprite_sheet = true;

	// Add the sheet as the first subimage.
	spr.Add(sheet);

	// Create sub-sprites from the sheet.
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			spr.Add(Drawing::Bitmap(spr.__frames[0], Rectangle(frame_width * j, frame_height * i, frame_width, frame_height)));

	// Return the new sprite.
	return spr;

}
Sprite Sprite::FromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color) {

	Sprite sprite = FromSpriteSheet(path, frame_width, frame_height, origin_x, origin_y);
	sprite.__frames[0].ConvertMaskToAlpha(alpha_color);

	/*for (size_t i = 1; i < sprite.__frames.size(); ++i)
		sprite.__frames[i].ConvertMaskToAlpha(alpha_color);*/

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

	return (__frames[__using_sprite_sheet] ? __frames[__using_sprite_sheet].Width() : 0);

}
unsigned int Sprite::Height() const {

	return (__frames[__using_sprite_sheet] ? __frames[__using_sprite_sheet].Height() : 0);

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

	__origin.SetXY(origin_x, origin_y);

}

ALLEGRO_BITMAP* Sprite::AlPtr() const {

	return (__frames.size() <= __using_sprite_sheet ? nullptr : __frames[__using_sprite_sheet].AlPtr());

}

void Sprite::Add(Drawing::Bitmap& subimage, bool owner) {

	if (owner)
		__frames.push_back(std::move(subimage));
	else
		__frames.push_back(subimage);

}
void Sprite::Add(const char* path) {

	Add(Drawing::Bitmap(path), true);

}

const Drawing::Bitmap& Sprite::Subimage(int subimage) const {

	int index = (std::abs)(subimage) % __frames.size();
	index += __using_sprite_sheet;

	return __frames[index];

}
const Drawing::Bitmap& Sprite::operator[] (int subimage) const {

	return Subimage(subimage);

}
