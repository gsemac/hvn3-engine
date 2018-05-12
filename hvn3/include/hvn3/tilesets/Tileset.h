#pragma once
#include "hvn3/graphics/Bitmap.h"
#include <vector>

namespace hvn3 {

	enum class TileFlags {
		AutoTile
	};

	class Tile {
	public:
		Tile(const Graphics::Bitmap& bitmap);
		TileFlags Flags() const;
	private:
		Graphics::Bitmap _bitmap;
		TileFlags _flags;
	};

	class Tileset {

	public:
		Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size);
		Tileset(const Graphics::Bitmap& tileset, int tile_width, int tile_height);
		Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size, const PointI& offset, const PointI& separation);
				
		const Graphics::Bitmap& At(unsigned int x, unsigned int y) const;
		const Graphics::Bitmap& At(unsigned int n) const;
		size_t Count() const;
		size_t Rows() const;
		size_t Columns() const;

		const Graphics::Bitmap& Bitmap() const;
		const SizeI& GridSize() const;
		
		//static Tileset GenerateAutoTiles(const Tileset& autotiles);

	private:
		Graphics::Bitmap _bitmap;
		std::vector<Graphics::Bitmap> _tiles;
		size_t _columns;
		SizeI _tile_size;

	};

}