#pragma once
#include "hvn3/tilesets/TileMap.h"

namespace hvn3 {

	enum AUTOTILE_INDEX : TileMap::tile_id {
		AUTOTILE_INDEX_ALL = 1, // surrounded
		AUTOTILE_INDEX_N,
		AUTOTILE_INDEX_W,
		AUTOTILE_INDEX_N_W,
		AUTOTILE_INDEX_N_W_NW,
		AUTOTILE_INDEX_E,
		AUTOTILE_INDEX_N_E,
		AUTOTILE_INDEX_N_E_NE, // bottom left corner
		AUTOTILE_INDEX_E_W, // horizontal bar
		AUTOTILE_INDEX_N_E_W,
		AUTOTILE_INDEX_N_E_W_NW,
		AUTOTILE_INDEX_N_E_W_NE,
		AUTOTILE_INDEX_N_E_W_NE_NW, // bottom edge
		AUTOTILE_INDEX_S,
		AUTOTILE_INDEX_N_S, // vertical bar
		AUTOTILE_INDEX_S_W,
		AUTOTILE_INDEX_N_S_W,
		AUTOTILE_INDEX_N_S_W_NW,
		AUTOTILE_INDEX_E_S,
		AUTOTILE_INDEX_N_E_S,
		AUTOTILE_INDEX_N_E_S_NE,
		AUTOTILE_INDEX_E_S_W,
		AUTOTILE_INDEX_N_E_S_W,
		AUTOTILE_INDEX_N_E_S_W_NW_SE,
		AUTOTILE_INDEX_N_E_S_W_NE,
		AUTOTILE_INDEX_N_E_S_W_NE_NW,
		AUTOTILE_INDEX_S_W_SW, // top right corner
		AUTOTILE_INDEX_N_S_W_SW,
		AUTOTILE_INDEX_N_S_W_NW_SW, // right edge
		AUTOTILE_INDEX_E_S_W_SW,
		AUTOTILE_INDEX_N_E_S_W_NE_SW,
		AUTOTILE_INDEX_N_E_S_W_NW_SW,
		AUTOTILE_INDEX_N_E_S_W_SW, 
		AUTOTILE_INDEX_N_E_S_W_NE_NW_SW,
		AUTOTILE_INDEX_E_S_SE, // top left corner
		AUTOTILE_INDEX_N_E_S_SE,
		AUTOTILE_INDEX_N_E_S_NE_SE, // left edge
		AUTOTILE_INDEX_E_S_W_SE,
		AUTOTILE_INDEX_N_E_S_W_SE,
		AUTOTILE_INDEX_N_E_S_W_NW_SE_2, // ? (duplicate)
		AUTOTILE_INDEX_N_E_S_W_NE_SE,
		AUTOTILE_INDEX_N_E_S_W_NW_NE_SE,
		AUTOTILE_INDEX_E_S_W_SW_SE, // top edge
		AUTOTILE_INDEX_N_E_S_W_SW_SE,
		AUTOTILE_INDEX_N_E_S_W_NW_SW_SE,
		AUTOTILE_INDEX_N_E_S_W_NE_SW_SE,
		AUTOTILE_INDEX_N_E_S_W_NW,
		AUTOTILE_INDEX_NONE
	};

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

			// This is not the most efficient means of finding the autotile index, although this probably won't ever be used in real-time.
			// #todo Make this better.

			if (n && e && w && s && ne && nw && se && sw)
				auto_tile_index = AUTOTILE_INDEX_ALL;
			else if (n && !e && !w && !s)
				auto_tile_index = AUTOTILE_INDEX_N;
			else if (!n && !e && w && !s)
				auto_tile_index = AUTOTILE_INDEX_W;
			else if (n && !e && w && !s && !nw)
				auto_tile_index = AUTOTILE_INDEX_N_W;
			else if (n && !e && w && !s && nw)
				auto_tile_index = AUTOTILE_INDEX_N_W_NW;
			else if (!n && e && !w && !s)
				auto_tile_index = AUTOTILE_INDEX_E;
			else if (n && e && !w && !s && !ne)
				auto_tile_index = AUTOTILE_INDEX_N_E;
			else if (n && e && !w && !s && ne)
				auto_tile_index = AUTOTILE_INDEX_N_E_NE;
			else if (!n && e && w && !s)
				auto_tile_index = AUTOTILE_INDEX_E_W;
			else if (n && e && w && !s && !nw && !ne)
				auto_tile_index = AUTOTILE_INDEX_N_E_W;
			else if (n && e && w && !s && nw && !ne)
				auto_tile_index = AUTOTILE_INDEX_N_E_W_NW;
			else if (n && e && w && !s && !nw && ne)
				auto_tile_index = AUTOTILE_INDEX_N_E_W_NE;
			else if (n && e && w && !s && nw && ne)
				auto_tile_index = AUTOTILE_INDEX_N_E_W_NE_NW;
			else if (!n && !e && !w && s)
				auto_tile_index = AUTOTILE_INDEX_S;
			else if (n && !e && !w && s)
				auto_tile_index = AUTOTILE_INDEX_N_S;
			else if (!n && !e && w && s && !sw)
				auto_tile_index = AUTOTILE_INDEX_S_W;
			else if (n && !e && w && s && !nw && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_S_W;
			else if (n && !e && w && s && nw && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_S_W_NW;
			else if (!n && e && !w && s && !se)
				auto_tile_index = AUTOTILE_INDEX_E_S;
			else if (n && e && !w && s && !ne && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S;
			else if (n && e && !w && s && ne && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_NE;
			else if (!n && e && w && s && !se && !sw)
				auto_tile_index = AUTOTILE_INDEX_E_S_W;
			else if (n && e && w && s && !ne && !nw && !se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W;
			else if (n && e && w && s && !ne && nw && se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NW_SE;
			else if (n && e && w && s && ne && !nw && !se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NE;
			else if (n && e && w && s && ne && nw && !se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NE_NW;
			else if (!n && !e && w && s && sw)
				auto_tile_index = AUTOTILE_INDEX_S_W_SW;
			else if (n && !e && w && s && !nw && sw)
				auto_tile_index = AUTOTILE_INDEX_N_S_W_SW;
			else if (n && !e && w && s && nw && sw)
				auto_tile_index = AUTOTILE_INDEX_N_S_W_NW_SW;
			else if (!n && e && w && s && sw && !se)
				auto_tile_index = AUTOTILE_INDEX_E_S_W_SW;
			else if (n && e && w && s && !nw && ne && sw && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NE_SW;
			else if (n && e && w && s && nw && !ne && sw && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NW_SW;
			else if (n && e && w && s && !nw && !ne && sw && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_SW;
			else if (n && e && w && s && ne && nw && sw && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NE_NW_SW;
			else if (!n && e && !w && s)
				auto_tile_index = AUTOTILE_INDEX_E_S_SE;
			else if (n && e && !w && s && !ne && se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_SE;
			else if (n && e && !w && s && ne && se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_NE_SE;
			else if (!n && e && w && s && se && !sw)
				auto_tile_index = AUTOTILE_INDEX_E_S_W_SE;
			else if (n && e && w && s && !ne && !nw && se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_SE;
			else if (n && e && w && s && ne && !nw && se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NE_SE;
			else if (n && e && w && s && ne && nw && se && !sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NW_NE_SE;
			else if (!n && e && w && s && se && sw)
				auto_tile_index = AUTOTILE_INDEX_E_S_W_SW_SE;
			else if (n && e && w && s && !ne && !nw && se && sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_SW_SE;
			else if (n && e && w && s && !ne && nw && se && sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NW_SW_SE;
			else if (n && e && w && s && ne && !nw && se && sw)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NE_SW_SE;
			else if (n && e && s && w && nw && !ne && !sw && !se)
				auto_tile_index = AUTOTILE_INDEX_N_E_S_W_NW;
			else
				auto_tile_index = AUTOTILE_INDEX_NONE;

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