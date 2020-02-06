#pragma once

#include "hvn3/math/Point2d.h"
#include "hvn3/tiles/tile_map.h"

#include <functional>
#include <vector>

namespace hvn3::tiles {

	class EdgeTileFinder {

	public:
		// Add some additional algorithms in the future?
		enum EdgeDetectionAlgorithm {
			// Considers any solid tile not fully surrounded by solid tiles to be an edge tile.
			Simple
		};

		void FindEdgeTiles(std::vector<PointI>& out, const TileMap& map, int layer, std::function<bool(TileMap::Tile)> is_solid) const;

	};

}