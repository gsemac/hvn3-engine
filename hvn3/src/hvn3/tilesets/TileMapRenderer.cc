#include "hvn3/exceptions/Exception.h"
#include "hvn3/tilesets/AutoTileMapper.h"
#include "hvn3/tilesets/TileMapRenderer.h"

namespace hvn3 {

	TileMapRenderer::TileMapRenderer() {

		/*	The default auto-tile layout expects the following configuration:
			A B
			C D
			E F
		*/

		SetAutoTileLayout(1, 0, 0, 1, 1, 1, 0, 2, 1, 2);

	}

	void TileMapRenderer::AddTileset(const Tileset& tileset) {
		_tilesets.push_back(tileset);
	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap) {

		// If there are no tilesets to use, do nothing.
		if (_tilesets.size() == 0)
			return;

		canvas.HoldBitmapDrawing(true);

		for (auto layer = tilemap.LayersBegin(); layer != tilemap.LayersEnd(); ++layer) {
			for (int i = 0; i < tilemap.Height(); ++i)
				for (int j = 0; j < tilemap.Width(); ++j) {

					TileMap::Tile tile_data = tilemap.At(i, j, layer->first);

					if (tile_data.id <= 0)
						continue;

					SizeI tile_size = _tilesets[0].GridSize();
					TileMap::tile_id auto_tile_index = tile_data.AutoTileIndex();

					if (auto_tile_index <= 0) {

						// If the tile is not an autotile, draw it normally.
						const Graphics::Bitmap& tile_image = _tilesets[0].At(tile_data.TileIndex() - 1).bitmap;
						canvas.DrawBitmap(i * tile_size.width, j * tile_size.height, tile_image);

					}
					else {

						// If the tile is an autotile, we need to draw it according to its autotile index.
						_renderAutoTile(canvas, tile_data, i, j);

						/*	const Graphics::Bitmap& tile_image = _tilesets[0].At(tile_data.TileIndex() - 1).bitmap;
							canvas.DrawBitmap(i * tile_size.width, j * tile_size.height, tile_image, Color::Red * 0.5f);*/

					}

				}
		}

		canvas.HoldBitmapDrawing(false);

	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y) {



	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region) {



	}
	void TileMapRenderer::SetAutoTileLayout(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t x4, uint8_t y4) {

		_auto_tile_layout[0] = Point2d<uint8_t>(x0, y0);
		_auto_tile_layout[1] = Point2d<uint8_t>(x1, y1);
		_auto_tile_layout[2] = Point2d<uint8_t>(x2, y2);
		_auto_tile_layout[3] = Point2d<uint8_t>(x3, y3);
		_auto_tile_layout[4] = Point2d<uint8_t>(x4, y4);

	}

	void TileMapRenderer::_renderAutoTile(Graphics::Graphics& canvas, TileMap::Tile tile_data, int i, int j) {

		TileMap::tile_id tile_index = tile_data.TileIndex() - 1;
		TileMap::tile_id auto_tile_index = tile_data.AutoTileIndex();
		SizeI tile_size = _tilesets[0].GridSize();

		TileMap::tile_id x = tile_index % _tilesets[0].Columns();
		TileMap::tile_id y = tile_index / _tilesets[0].Columns();

		int dx = i * tile_size.width;
		int dy = j * tile_size.height;

		switch (auto_tile_index) {
		case AUTOTILE_INDEX_ALL:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
			break;
		case AUTOTILE_INDEX_N:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 2).bitmap, RectangleF(0, 0, 16, 32));
			canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(16, 0, 16, 32));
			break;
		case AUTOTILE_INDEX_W:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 0, 32, 16));
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 16, 32, 16));
			break;
		case AUTOTILE_INDEX_N_W:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y + 2).bitmap);
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 0, 16, 16));
			break;
		case AUTOTILE_INDEX_N_W_NW:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y + 2).bitmap);
			break;
		case AUTOTILE_INDEX_E:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(0, 0, 32, 16));
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(0, 16, 32, 16));
			break;
		case AUTOTILE_INDEX_N_E:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 2).bitmap);
			canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(16, 0, 16, 16));
			break;
		case AUTOTILE_INDEX_N_E_NE:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 2).bitmap);
			break;
		case AUTOTILE_INDEX_E_W:
		case AUTOTILE_INDEX_E_S_W:
		case AUTOTILE_INDEX_E_S_W_SW:
		case AUTOTILE_INDEX_E_S_W_SE:
		case AUTOTILE_INDEX_E_S_W_SW_SE:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 0, 16, 16));
			canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 0, 16, 16));
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 16, 16, 16));
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_E_S_W:
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 16, 32, 16));
				break;
			case AUTOTILE_INDEX_E_S_W_SW:
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(16, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_E_S_W_SE:
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_E_S_W_SW_SE:
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_N_E_W:
		case AUTOTILE_INDEX_N_E_W_NW:
		case AUTOTILE_INDEX_N_E_W_NE:
		case AUTOTILE_INDEX_N_E_W_NE_NW:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 0, 32, 16));
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 16, 16, 16));
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_N_E_W_NW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_W_NE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_W_NE_NW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_S:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(0, 0, 16, 32));
			canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(16, 0, 16, 32));
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
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(0, 16, 16, 16));
			canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(16, 16, 16, 16));
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(0, 0, 16, 16));
			canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(16, 0, 16, 16));
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_N_S_W:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 0, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_S_W_NW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(16, 0, 16, 32));
				break;
			case AUTOTILE_INDEX_N_E_S_NE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(16, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_S_W_SW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 0, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_S_W_NW_SW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_SE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_NE_SE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_S_W:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y + 1).bitmap);
			canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(0, 16, 16, 16));
			break;
		case AUTOTILE_INDEX_E_S:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap);
			canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y).bitmap, RectangleF(16, 16, 16, 16));
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
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y).bitmap);
			switch (auto_tile_index) {
			case AUTOTILE_INDEX_N_E_S_W_NW_SE:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_NW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_SW:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW_SW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_SW:
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_NW_SW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_SE:
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_SE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW_NE_SE:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_SW_SE:
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW_SW_SE:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NE_SW_SE:
				canvas.DrawBitmap(dx + 16, dy, _tilesets[0].At(x + 1, y + 1).bitmap, RectangleF(0, 16, 16, 16));
				canvas.DrawBitmap(dx, dy + 16, _tilesets[0].At(x, y + 2).bitmap, RectangleF(16, 0, 16, 16));
				canvas.DrawBitmap(dx + 16, dy + 16, _tilesets[0].At(x + 1, y + 2).bitmap, RectangleF(0, 0, 16, 16));
				break;
			case AUTOTILE_INDEX_N_E_S_W_NW:
				canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap, RectangleF(16, 16, 16, 16));
				break;
			}
			break;
		case AUTOTILE_INDEX_S_W_SW:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x + 1, y + 1).bitmap);
			break;
		case AUTOTILE_INDEX_E_S_SE:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y + 1).bitmap);
			break;
		case AUTOTILE_INDEX_NONE:
			canvas.DrawBitmap(dx, dy, _tilesets[0].At(x, y).bitmap);
			break;
		}

	}

}