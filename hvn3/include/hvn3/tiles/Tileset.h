#pragma once

#include "hvn3/graphics/Bitmap.h"

#include <vector>

namespace hvn3::tiles {

	class Tileset {

	public:
		class Tile {

		public:
			Tile(Graphics::Bitmap&& image);

			const Graphics::Bitmap& Image() const;

		private:
			Graphics::Bitmap image;

		};

		using size_type = size_t;

		Tileset(const Graphics::Bitmap& image, const SizeI& tileSize);
		Tileset(const Graphics::Bitmap& image, const SizeI& tileSize, const PointI& offset, const PointI& separation);
		Tileset(const Graphics::Bitmap& image, int tileWidth, int tileHeight);

		//Tile& TileAt(int x, int y);
		//Tile& TileAt(int n);
		const Tile& GetTile(int x, int y) const;
		const Tile& GetTile(int n) const;

		size_type Rows() const;
		size_type Columns() const;

		size_type Count() const;

		const Graphics::Bitmap& Image() const;

		const SizeI& Size() const;
		const SizeI& TileSize() const;

		//static Tileset FromFile(const std::string& file);
		//static Tileset FromFile(const std::string& file, const SizeI& tileSize);

	private:
		Graphics::Bitmap image;
		std::vector<Tile> tiles;
		size_type rows;
		SizeI tileSize;

	};

}