#ifndef __SPRITE_H
#define __SPRITE_H
#include <allegro5/allegro.h>
#include <vector>
#include <string>
#include "Point.h"
#include "Color.h"
#include "Bitmap.h"

class Sprite {

public:
	Sprite(const char* path);
	Sprite(const char* path, int origin_x, int origin_y);
	Sprite(const char* path, int origin_x, int origin_y, const Color& alpha_color);
	Sprite(const std::string& path);
	Sprite(const std::string& path, int origin_x, int origin_y);
	Sprite(const std::string& path, int origin_x, int origin_y, const Color& alpha_color);
	Sprite(Sprite&& other);
	~Sprite();

	static Sprite FromSpriteSheet(const char* path, int frame_width, int frame_height);
	static Sprite FromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y);
	static Sprite FromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color);
	static Sprite FromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y);
	static Sprite FromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color);
	static Sprite FromSpriteSheet(const std::string& path, int frame_width, int frame_height);
	static Sprite FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y);
	static Sprite FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color);
	static Sprite FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y);
	static Sprite FromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color);

	unsigned int Width() const;
	unsigned int Height() const;
	// If loaded from a sprite sheet, returns the length of each strip. Otherwise, returns the number of subimages.
	unsigned int StripLength() const;
	// Returns the number of subimages.
	unsigned int Length() const;
	// Returns a point corresponding to the origin (offset) of the sprite.
	Point Origin() const;
	// Sets the origin (offset) of the sprite to specified values.
	void SetOrigin(float origin_x, float origin_y);

	void Add(Drawing::Bitmap& subimage, bool owner = true);
	void Add(const char* path);

	ALLEGRO_BITMAP* AlPtr() const;

	const Drawing::Bitmap& Subimage(int subimage) const;
	const Drawing::Bitmap& operator[](int subimage) const;

private:
	std::vector<Drawing::Bitmap> __frames;
	Point __origin;
	bool __using_sprite_sheet;
	unsigned int __strip_length;

	Sprite();

};

#endif