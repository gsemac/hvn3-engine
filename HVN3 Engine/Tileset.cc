#include "Tileset.h"

Tileset::Tileset(const char* filepath, unsigned int tile_width, unsigned int tile_height) :
	Tileset(filepath, tile_width, tile_height, 0, 0, 0, 0) {
}
Tileset::Tileset(const char* filepath, unsigned int tile_width, unsigned int tile_height, unsigned int offset_x, unsigned int offset_y, unsigned int separation_x, unsigned int separation_y) :
	_bitmap(filepath) {

	// Initialize row count.
	_rows = 0;

	// Generate a vector of sub-bitmaps to represent each tile.
	for (unsigned int y = offset_y; y < _bitmap.Height(); y += tile_height + separation_y) {

		// Insert the next row of tiles.
		for (unsigned int x = offset_x; x < _bitmap.Width(); x += tile_width + separation_x)
			_tiles.push_back(Drawing::Bitmap(_bitmap, Rectangle(x, y, Min(tile_width, _bitmap.Width() - x), Min(tile_height, _bitmap.Height() - y))));

		// Increment the number of rows.
		++_rows;

	}

}

const Drawing::Bitmap& Tileset::TileAt(unsigned int x, unsigned int y) const {

	return _tiles[y * Columns() + x];

}
const Drawing::Bitmap& Tileset::TileAt(unsigned int n) const {

	return _tiles[n];

}
size_t Tileset::TileCount() const {

	return _tiles.size();

}
size_t Tileset::Rows() const {

	return _rows;

}
size_t Tileset::Columns() const {

	return _tiles.size() / _rows;

}