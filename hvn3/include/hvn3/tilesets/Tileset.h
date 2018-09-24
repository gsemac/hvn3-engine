#pragma once
#include "hvn3/graphics/Bitmap.h"
#include <vector>

namespace hvn3 {

	class Tileset {

	public:
		class Tile {

		public:
			Tile(Graphics::Bitmap&& bitmap);

			Graphics::Bitmap bitmap;
			int flag;

		};

		Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size);
		Tileset(const Graphics::Bitmap& tileset, int tile_width, int tile_height);
		Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size, const PointI& offset, const PointI& separation);

		const Tile& At(unsigned int x, unsigned int y) const;
		const Tile& At(unsigned int n) const;
		Tile& GetAt(unsigned int x, unsigned int y);
		Tile& GetAt(unsigned int n);
		size_t Count() const;
		size_t Rows() const;
		size_t Columns() const;

		const Graphics::Bitmap& Bitmap() const;
		const SizeI& GridSize() const;
		const SizeI& TileSize() const;

		static Tileset FromFile(const std::string& file);
		static Tileset FromFile(const std::string& file, const SizeI& tile_size);

		//static Tileset GenerateAutoTiles(const Tileset& autotiles);

	private:
		Graphics::Bitmap _bitmap;
		std::vector<Tile> _tiles;
		size_t _rows;
		SizeI _tile_size;

	};

}