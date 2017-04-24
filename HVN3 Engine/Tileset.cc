#include "Tileset.h"

Tileset::Tileset(const char* filepath, int tile_width, int tile_height) :
	Tileset(filepath, tile_width, tile_height, 0, 0, 0, 0) {}
Tileset::Tileset(const char* filepath, int tile_width, int tile_height, int offset_x, int offset_y, int separation_x, int separation_y) :
	_bitmap(filepath) {



}

const Drawing::Bitmap Tileset::TileAt(int x, int y) const {

	return Drawing::Bitmap(_bitmap);

}