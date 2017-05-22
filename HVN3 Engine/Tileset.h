#pragma once
#include "Bitmap.h"
#include <vector>

namespace hvn3 {

	class Tileset {

	public:
		Tileset(const char* filepath, unsigned int tile_width, unsigned int tile_height);
		Tileset(const char* filepath, unsigned int tile_width, unsigned int tile_height, unsigned int offset_x, unsigned int offset_y, unsigned int separation_x, unsigned int separation_y);

		const Drawing::Bitmap& TileAt(unsigned int x, unsigned int y) const;
		const Drawing::Bitmap& TileAt(unsigned int n) const;
		size_t TileCount() const;
		size_t Rows() const;
		size_t Columns() const;

	private:
		Drawing::Bitmap _bitmap;
		std::vector<Drawing::Bitmap> _tiles;
		size_t _rows;

	};

}