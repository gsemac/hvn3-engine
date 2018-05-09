#include "hvn3/tilesets/Tileset.h"

namespace hvn3 {

	Tileset::Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size) :
		Tileset(tileset, tile_size, PointI(0, 0), PointI(0, 0)) {
	}
	Tileset::Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size, const PointI& offset, const PointI& separation) :
		_bitmap(tileset),
		_tile_size(tile_size) {

		// Initialize row count.
		_rows = 0;

		// Get width/height as unsigned integers.
		int w = tile_size.Width();
		int h = tile_size.Height();

		// Generate a vector of sub-bitmaps to represent each tile.
		for (int y = offset.Y(); y < _bitmap.Height(); y += h + separation.Y()) {

			// Insert the next row of tiles.
			for (int x = offset.X(); x < _bitmap.Width(); x += w + separation.X())
				_tiles.push_back(Graphics::Bitmap(_bitmap, RectangleI(x, y, Math::Min(w, _bitmap.Width() - x), Math::Min(h, _bitmap.Height() - y))));

			// Increment the number of rows.
			++_rows;

		}

	}

	const Graphics::Bitmap& Tileset::TileAt(unsigned int x, unsigned int y) const {

		return _tiles[y * Columns() + x];

	}
	const Graphics::Bitmap& Tileset::TileAt(unsigned int n) const {

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

	const Graphics::Bitmap& Tileset::Bitmap() const {

		return _bitmap;

	}
	const SizeI& Tileset::TileSize() const {

		return _tile_size;

	}

}