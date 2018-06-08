#pragma once
#include "hvn3/tilesets/TileMap.h"

namespace hvn3 {

	class AutoTileMapper {

	public:
		struct AutoTileData {

			AutoTileData() {
				northWest = false;
				north = false;
				northEast = false;
				west = false;
				east = false;
				southWest = false;
				south = false;
				southEast = false;
			}
			AutoTileData(TileMap::tile_id auto_tile_index) {
				northWest = auto_tile_index & 1u;
				north = auto_tile_index & 2u;
				northEast = auto_tile_index & 4u;
				west = auto_tile_index & 8u;
				east = auto_tile_index & 16u;
				southWest = auto_tile_index & 32u;
				south = auto_tile_index & 64u;
				southEast = auto_tile_index & 128u;
			}

			bool northWest;
			bool north;
			bool northEast;
			bool west;
			bool east;
			bool southWest;
			bool south;
			bool southEast;

		};

		AutoTileMapper(TileMap& map) {
			_map = &map;
		}

		TileMap::tile_id CalculateAutoTileIndex(int x, int y, TileMap::layer_id depth) const {

			// Get the tile data.
			TileMap::Tile tile = _map->At(x, y, depth);
			TileMap::tile_id tile_index = tile.TileIndex();

			// Check surrounding tiles for the same tile index.
			bool n = _map->Exists(x, y - 1, depth, tile_index);
			bool e = _map->Exists(x + 1, y, depth, tile_index);
			bool w = _map->Exists(x - 1, y, depth, tile_index);
			bool s = _map->Exists(x, y + 1, depth, tile_index);
			bool ne = _map->Exists(x + 1, y - 1, depth, tile_index);
			bool nw = _map->Exists(x - 1, y - 1, depth, tile_index);
			bool se = _map->Exists(x + 1, y + 1, depth, tile_index);
			bool sw = _map->Exists(x - 1, y + 1, depth, tile_index);

			TileMap::tile_id auto_tile_index = 0;

			if (n && e && w && s && ne && nw && se && sw)
				auto_tile_index = 1;
			else if (n && !e && !w && !s)
				auto_tile_index = 2;
			else if (w && !n && !e && !s)
				auto_tile_index = 3;
			else if (w && n && !e && !s && !nw)
				auto_tile_index = 4;
			else if (n) {
				if (s)
					auto_tile_index = 15;
			}

			return auto_tile_index;

		}

		void ApplyAutoTilingAt(int x, int y, TileMap::layer_id depth) {

			_applyAutoMappingAt(x, y, depth);

			// Surrounding tiles may need to be updated.
			if (_map->Exists(x - 1, y - 1, depth))
				_applyAutoMappingAt(x - 1, y - 1, depth); // nw
			if (_map->Exists(x, y - 1, depth))
				_applyAutoMappingAt(x, y - 1, depth); // n
			if (_map->Exists(x + 1, y - 1, depth))
				_applyAutoMappingAt(x + 1, y - 1, depth); // ne
			if (_map->Exists(x + 1, y, depth))
				_applyAutoMappingAt(x + 1, y, depth); // e
			if (_map->Exists(x + 1, y + 1, depth))
				_applyAutoMappingAt(x + 1, y + 1, depth); // se
			if (_map->Exists(x, y + 1, depth))
				_applyAutoMappingAt(x, y + 1, depth); // s
			if (_map->Exists(x - 1, y + 1, depth))
				_applyAutoMappingAt(x - 1, y + 1, depth); // sw
			if (_map->Exists(x - 1, y, depth))
				_applyAutoMappingAt(x - 1, y, depth); // w

		}

	private:
		TileMap* _map;

		void _applyAutoMappingAt(int x, int y, TileMap::layer_id depth) {

			// Generate the autotile index for the tile at this position.
			TileMap::Tile tile = _map->At(x, y, depth);
			TileMap::tile_id auto_tile_index = CalculateAutoTileIndex(x, y, depth);

			tile.SetAutoTileIndex(auto_tile_index);

			// Apply the autotile index to the tile.
			_map->SetTile(x, y, tile.id, depth);

		}

	};

}