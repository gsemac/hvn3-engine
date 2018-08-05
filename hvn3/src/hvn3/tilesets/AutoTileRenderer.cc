#include "hvn3/tilesets/AutoTileRenderer.h"

namespace hvn3 {

	AutoTileRenderer::AutoTileRenderer() {

		/*	The default auto-tile layout expects the following configuration:
		A B
		C D
		E F
		*/
		SetAutoTileLayout(1, 0, 0, 1, 1, 1, 0, 2, 1, 2);

	}

	void AutoTileRenderer::SetAutoTileLayout(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t x4, uint8_t y4) {

		_auto_tile_layout[0] = Point2d<uint8_t>(x0, y0);
		_auto_tile_layout[1] = Point2d<uint8_t>(x1, y1);
		_auto_tile_layout[2] = Point2d<uint8_t>(x2, y2);
		_auto_tile_layout[3] = Point2d<uint8_t>(x3, y3);
		_auto_tile_layout[4] = Point2d<uint8_t>(x4, y4);

	}
	void AutoTileRenderer::ApplyAutoTilingAt(TileMap& map, int x, int y, TileMap::layer_id depth) {

		_applyAutoMappingAt(map, x, y, depth);
		_applyAutoTileSurrounding(map, x, y, depth);

	}
	void AutoTileRenderer::ApplyAutoTiling(TileMap& map, TileMap::layer_id depth) {

		for (int y = 0; y < map.Height(); ++y)
			for (int x = 0; x < map.Width(); ++x)
				_applyAutoMappingAt(map, x, y, depth);

	}
	void AutoTileRenderer::DrawAutoTile(Graphics::Graphics& canvas, TileMap::Tile tile_data, const Tileset& tileset, float draw_x, float draw_y) {

		TileMap::tile_id tile_index = tile_data.TileIndex() - 1;
		TileMap::tile_id auto_tile_index = tile_data.AutoTileIndex();

		// These are the actual coordinates where the tile is drawn.
		float dx = draw_x;
		float dy = draw_y;

		// These are the coordinates of the tile graphic in the tileset.
		TileMap::tile_id x = tile_index % tileset.Columns();
		TileMap::tile_id y = tile_index / tileset.Columns();

		// If this is not an auto-tile, draw it without any extra processing.
		if (tileset.At(tile_index).flags != Tileset::TileFlags::AutoTile) {
			canvas.DrawBitmap(dx, dy, tileset.At(tile_index).bitmap);
			return;
		}

		switch (auto_tile_index) {
		case AUTOTILE_INDEX_ALL:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
			break;
		case AUTOTILE_INDEX_N:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 2).bitmap, RectangleF(0, 0, 16, 32));
			canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 2).bitmap, RectangleF(16, 0, 16, 32));
			break;
		case AUTOTILE_INDEX_W:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 0, 32, 16));
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 16, 32, 16));
			break;
		case AUTOTILE_INDEX_N_W:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y + 2).bitmap);
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y).bitmap, RectangleF(0, 0, 16, 16));
			break;
		case AUTOTILE_INDEX_N_W_NW:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y + 2).bitmap);
			break;
		case AUTOTILE_INDEX_E:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(0, 0, 32, 16));
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(0, 16, 32, 16));
			break;
		case AUTOTILE_INDEX_N_E:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 2).bitmap);
			canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y).bitmap, RectangleF(16, 0, 16, 16));
			break;
		case AUTOTILE_INDEX_N_E_NE:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 2).bitmap);
			break;
		case AUTOTILE_INDEX_E_W:
		case AUTOTILE_INDEX_E_S_W:
		case AUTOTILE_INDEX_E_S_W_SW:
		case AUTOTILE_INDEX_E_S_W_SE:
		case AUTOTILE_INDEX_E_S_W_SW_SE:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 0, 16, 16));
			canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 0, 16, 16));
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 16, 16, 16));
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_E_S_W:
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(0, 16, 32, 16));
				break;
			case AUTOTILE_INDEX_E_S_W_SW:
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(16, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_E_S_W_SE:
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_E_S_W_SW_SE:
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_N_E_W:
		case AUTOTILE_INDEX_N_E_W_NW:
		case AUTOTILE_INDEX_N_E_W_NE:
		case AUTOTILE_INDEX_N_E_W_NE_NW:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y).bitmap, RectangleF(0, 0, 32, 16));
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 16, 16, 16));
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_N_E_W_NW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_W_NE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_W_NE_NW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_S:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(0, 0, 16, 32));
			canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(16, 0, 16, 32));
			break;
		case AUTOTILE_INDEX_N_S:
		case AUTOTILE_INDEX_N_S_W:
		case AUTOTILE_INDEX_N_S_W_NW:
		case AUTOTILE_INDEX_N_E_S:
		case AUTOTILE_INDEX_N_E_S_NE:
		case AUTOTILE_INDEX_N_S_W_SW:
		case AUTOTILE_INDEX_N_S_W_NW_SW:
		case AUTOTILE_INDEX_N_E_S_SE:
		case AUTOTILE_INDEX_N_E_S_NE_SE:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(0, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(0, 0, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(16, 0, 16, 16));
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_N_S_W:
				canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y).bitmap, RectangleF(0, 0, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_S_W_NW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y).bitmap, RectangleF(16, 0, 16, 32));
				break;
			case AUTOTILE_INDEX_N_E_S_NE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(16, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_S_W_SW:
				canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y).bitmap, RectangleF(0, 0, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_S_W_NW_SW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_SE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_NE_SE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_S_W:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y + 1).bitmap);
			canvas.DrawBitmap(dx, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
			break;
		case AUTOTILE_INDEX_E_S:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap);
			canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y).bitmap, RectangleF(16, 16, 16, 16));
			break;
		case AUTOTILE_INDEX_N_E_S_W:
		case AUTOTILE_INDEX_N_E_S_W_NW_SE:
		case AUTOTILE_INDEX_N_E_S_W_NE:
		case AUTOTILE_INDEX_N_E_S_W_NE_NW:
		case AUTOTILE_INDEX_N_E_S_W_NE_SW:
		case AUTOTILE_INDEX_N_E_S_W_NW_SW:
		case AUTOTILE_INDEX_N_E_S_W_SW:
		case AUTOTILE_INDEX_N_E_S_W_NE_NW_SW:
		case AUTOTILE_INDEX_N_E_S_W_SE:
		case AUTOTILE_INDEX_N_E_S_W_NE_SE:
		case AUTOTILE_INDEX_N_E_S_W_NW_NE_SE:
		case AUTOTILE_INDEX_N_E_S_W_SW_SE:
		case AUTOTILE_INDEX_N_E_S_W_NW_SW_SE:
		case AUTOTILE_INDEX_N_E_S_W_NE_SW_SE:
		case AUTOTILE_INDEX_N_E_S_W_NW:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y).bitmap);
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_N_E_S_W_NW_SE:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_NW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_SW:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW_SW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_SW:
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_NW_SW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_SE:
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_SE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW_NE_SE:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_SW_SE:
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW_SW_SE:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_SW_SE:
				canvas.DrawBitmap(dx + 16, dy, tileset.At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, tileset.At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, tileset.At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW:
				canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_S_W_SW:
			canvas.DrawBitmap(dx, dy, tileset.At(x + 1, y + 1).bitmap);
			break;
		case AUTOTILE_INDEX_E_S_SE:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y + 1).bitmap);
			break;
		case AUTOTILE_INDEX_NONE:
			canvas.DrawBitmap(dx, dy, tileset.At(x, y).bitmap);
			break;
		}

	}

	AutoTileRenderer::AUTOTILE_INDEX AutoTileRenderer::ComputeAutoTileIndex(TileMap& map, int x, int y, TileMap::layer_id depth) {

		// Get the tile data.
		TileMap::Tile tile = map.At(x, y, depth);
		TileMap::tile_id tile_index = tile.TileIndex();
	
		// Check surrounding tiles for the same tile index.
		bool n = map.Exists(x, y - 1, depth, tile_index);
		bool e = map.Exists(x + 1, y, depth, tile_index);
		bool w = map.Exists(x - 1, y, depth, tile_index);
		bool s = map.Exists(x, y + 1, depth, tile_index);
		bool ne = map.Exists(x + 1, y - 1, depth, tile_index);
		bool nw = map.Exists(x - 1, y - 1, depth, tile_index);
		bool se = map.Exists(x + 1, y + 1, depth, tile_index);
		bool sw = map.Exists(x - 1, y + 1, depth, tile_index);

		AUTOTILE_INDEX auto_tile_index = static_cast<AUTOTILE_INDEX>(0);

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

	void AutoTileRenderer::_applyAutoMappingAt(TileMap& map, int x, int y, TileMap::layer_id depth) {

		// Generate the autotile index for the tile at this position.
		TileMap::Tile tile = map.At(x, y, depth);
		TileMap::tile_id auto_tile_index = ComputeAutoTileIndex(map, x, y, depth);

		tile.SetAutoTileIndex(auto_tile_index);

		// Apply the autotile index to the tile.
		map.SetTile(x, y, tile.id, depth);

	}
	void AutoTileRenderer::_applyAutoTileSurrounding(TileMap& map, int x, int y, TileMap::layer_id depth) {

		// Surrounding tiles may need to be updated.
		if (map.Exists(x - 1, y - 1, depth))
			_applyAutoMappingAt(map, x - 1, y - 1, depth); // nw
		if (map.Exists(x, y - 1, depth))
			_applyAutoMappingAt(map, x, y - 1, depth); // n
		if (map.Exists(x + 1, y - 1, depth))
			_applyAutoMappingAt(map, x + 1, y - 1, depth); // ne
		if (map.Exists(x + 1, y, depth))
			_applyAutoMappingAt(map, x + 1, y, depth); // e
		if (map.Exists(x + 1, y + 1, depth))
			_applyAutoMappingAt(map, x + 1, y + 1, depth); // se
		if (map.Exists(x, y + 1, depth))
			_applyAutoMappingAt(map, x, y + 1, depth); // s
		if (map.Exists(x - 1, y + 1, depth))
			_applyAutoMappingAt(map, x - 1, y + 1, depth); // sw
		if (map.Exists(x - 1, y, depth))
			_applyAutoMappingAt(map, x - 1, y, depth); // w

	}

}