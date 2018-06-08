#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/tilesets/Tileset.h"
#include <array>
#include <cstddef>
#include <initializer_list>
#include <vector>

namespace hvn3 {

	class TileMapRenderer {

	public:
		TileMapRenderer();

		void AddTileset(const Tileset& tileset);

		void Render(Graphics::Graphics& canvas, const TileMap& tilemap);
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y);
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region);

		void SetAutoTileLayout(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t x4, uint8_t y4);

	private:
		std::vector<Tileset> _tilesets;
		// Autotiles are in sets of 6. For a given autotile, these 5 points define the offsets to each of the remaining autotiles.
		std::array<Point2d<uint8_t>, 5> _auto_tile_layout;

		void _renderAutoTile(Graphics::Graphics& canvas, TileMap::Tile tile_data, int i, int j);
		
	};

}