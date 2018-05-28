#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/tilesets/Tileset.h"
#include <vector>

namespace hvn3 {

	class TileMapRenderer {

	public:
		TileMapRenderer();

		void AddTileset(const Tileset& tileset);
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap);
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y);
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region);

	private:
		std::vector<Tileset> _tilesets;

	};

}