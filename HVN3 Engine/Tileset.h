#pragma once
#include "Bitmap.h"
#include <vector>

namespace hvn3 {

	class Tileset {

	public:
		Tileset(const char* filepath, const SizeI& tile_size);
		Tileset(const char* filepath, const SizeI& tile_size, const PointI& offset, const PointI& separation);
				
		const Drawing::Bitmap& TileAt(unsigned int x, unsigned int y) const;
		const Drawing::Bitmap& TileAt(unsigned int n) const;
		size_t TileCount() const;
		size_t Rows() const;
		size_t Columns() const;

		const Drawing::Bitmap& Bitmap() const;
		const SizeI& TileSize() const;
		
	private:
		Drawing::Bitmap _bitmap;
		std::vector<Drawing::Bitmap> _tiles;
		size_t _rows;
		SizeI _tile_size;

	};

}