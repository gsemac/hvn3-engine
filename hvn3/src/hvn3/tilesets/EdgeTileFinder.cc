#include "hvn3/tilesets/EdgeTileFinder.h"

namespace hvn3 {

	void EdgeTileFinder::FindEdgeTiles(std::vector<PointI>& out, const TileMap& map, TileMap::layer_id layer, std::function<bool(TileMap::Tile)> is_solid) const {

		// If a tile is not entirely surrounded, then consider it an outer edge.
		// This will, of course, result in some tiles being flagged that shouldn't be, but that should be negligible for most maps.

		std::vector<TileMap::Tile> neighbors;

		for (auto y = 0; y < map.Rows(); ++y)
			for (auto x = 0; x < map.Columns(); ++x) {

				TileMap::Tile tile = map.At(x, y, layer);

				if (!is_solid(tile))
					continue;

				neighbors.clear();
				map.GetNeighborsOf(neighbors, x, y, layer);

				// We ignore diagonals by looking at every other neighbor (clockwise from left).

				for (size_t i = 0; i < neighbors.size(); i += 2) {

					if (!is_solid(neighbors[i])) {

						out.push_back(PointI(x, y));
						break;

					}

				}

			}



	}


}