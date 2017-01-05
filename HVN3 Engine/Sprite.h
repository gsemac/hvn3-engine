#ifndef __SPRITE_H
#define __SPRITE_H
#include <allegro5/allegro.h>
#include <vector>
#include "Point.h"

class Sprite {

public:
	Sprite(const char* filename, int origin_x = 0, int origin_y = 0);
	Sprite(Sprite&& other);
	~Sprite();

	static Sprite FromSpriteSheet(const char* filename, int frame_width, int frame_height, int origin_x = 0, int origin_y = 0);

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