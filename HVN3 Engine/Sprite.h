#ifndef __SPRITE_H
#define __SPRITE_H
#include <allegro5/allegro.h>
#include <vector>
#include <string>
#include "Point.h"
#include "Color.h"

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

	void Add(ALLEGRO_BITMAP* bitmap_frame);
	void Add(const char* filename);

	ALLEGRO_BITMAP* AlPtr() const;
	ALLEGRO_BITMAP* AlPtr(int frame) const;
	
	ALLEGRO_BITMAP* operator[] (const int frame) const;

private:
	std::vector<ALLEGRO_BITMAP*> __frames;
	ALLEGRO_BITMAP* __sheet;
	unsigned int __width, __height;
	Point __origin;
	unsigned int __strip_length;

	Sprite();

};

#endif