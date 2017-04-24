#pragma once
#include "Bitmap.h"

class Tileset {

public:
	Tileset(const char* filepath, int tile_width, int tile_height);
	Tileset(const char* filepath, int tile_width, int tile_height, int offset_x, int offset_y, int separation_x, int separation_y);

	const Drawing::Bitmap TileAt(int x, int y) const;

private:
	Drawing::Bitmap _bitmap;

};