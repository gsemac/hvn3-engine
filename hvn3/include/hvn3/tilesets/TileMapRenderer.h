#pragma once
#include "hvn3/tilesets/AutoTileRenderer.h"
#include "hvn3/tilesets/Tileset.h"
#include <vector>

namespace hvn3 {

	class TileMapRenderer {

	public:
		TileMapRenderer(const SizeI& tile_size);

		void AddTileset(const Tileset& tileset);

		void Draw(Graphics::Graphics& canvas, const TileMap& tilemap);
		void Draw(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y);
		void Draw(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region);

		const std::vector<Tileset>& Tilesets() const;	

	private:
		std::vector<Tileset> _tilesets;
		SizeI _tile_size;
		AutoTileRenderer _auto_tile_renderer;
		
	};

}