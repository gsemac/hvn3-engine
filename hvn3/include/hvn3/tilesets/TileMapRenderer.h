#pragma once
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/tilesets/Tileset.h"
#include <vector>

namespace hvn3 {

	class TileMapRenderer {

		struct tileset_element_type {

		};

	public:
		TileMapRenderer();

		void AddTileset(const Tileset& tileset);

	private:
		std::vector<tileset_element_type> _tilesets;

	};

}