#include "hvn3/exceptions/Exception.h"
#include "hvn3/tilesets/AutoTileRenderer.h"
#include "hvn3/tilesets/TileMapRenderer.h"

namespace hvn3 {

	TileMapRenderer::TileMapRenderer() {}

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
						canvas.DrawBitmap(static_cast<float>(i * tile_size.width), static_cast<float>(j * tile_size.height), tile_image);

					}
					else {

						// If the tile is an autotile, we need to draw it according to its autotile index.
						_auto_tile_renderer.DrawAutoTile(canvas, tile_data, _tilesets[0], i * tile_size.width, j * tile_size.height);

					}

				}
		}

		canvas.HoldBitmapDrawing(false);

	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y) {



	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region) {



	}
	const std::vector<Tileset>& TileMapRenderer::Tilesets() const {
		return _tilesets;
	}

}