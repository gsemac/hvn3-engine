#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/tilesets/Tileset.h"
#include <array>
#include <cstddef>

namespace hvn3 {

	class AutoTileRenderer {

	public:
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

		AutoTileRenderer();

		// Sets the layout used to determine where related tiles are relative to the primary tile when drawing auto-tiles.
		void SetAutoTileLayout(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t x4, uint8_t y4);
		// Applies an auto-tile index to the tile at the given position, and updates surrounding tiles accordingly.
		void ApplyAutoTilingAt(TileMap& map, int x, int y, TileMap::layer_id depth);
		// Applies an auto-tile index to all tiles in the map.
		void ApplyAutoTiling(TileMap& map, TileMap::layer_id depth);
		void DrawAutoTile(Graphics::Graphics& canvas, TileMap::Tile tile_data, const Tileset& tileset, float draw_x, float draw_y);

		// Computes the auto-tile index for the tile at the given position and returns the result.
		static AUTOTILE_INDEX ComputeAutoTileIndex(TileMap& map, int x, int y, TileMap::layer_id depth);

	private:
		// Autotiles are in sets of 6. For a given autotile, these 5 points define the offsets to each of the remaining autotiles.
		std::array<Point2d<uint8_t>, 5> _auto_tile_layout;

		void _applyAutoMappingAt(TileMap& map, int x, int y, TileMap::layer_id depth);
		// Applies auto-tile indices to the tiles surrounding the tile at the given position, without affecting the given tile.
		void _applyAutoTileSurrounding(TileMap& map, int x, int y, TileMap::layer_id depth);

	};

}