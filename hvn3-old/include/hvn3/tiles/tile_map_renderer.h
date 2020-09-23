#pragma once

#include "hvn3/tiles/AutoTileRenderer.h"
#include "hvn3/tiles/tileset.h"
#include "hvn3/utility/Size.h"

#include <vector>

namespace hvn3::tiles {

	class TileMapRenderer {

	public:
		TileMapRenderer();

		void AddTileset(const Tileset& tileset);

		void DrawTiles(Graphics::Graphics& canvas, const TileMap& tileMap, const SizeI& tileSize);
		void DrawTiles(Graphics::Graphics& canvas, const TileMap& tileMap, const SizeI& tileSize, float x, float y);
		void DrawTiles(Graphics::Graphics& canvas, const TileMap& tileMap, const SizeI& tileSize, const RectangleF& region);

	private:
		std::vector<Tileset> tilesets;
		//AutoTileRenderer _auto_tile_renderer;

	};

}