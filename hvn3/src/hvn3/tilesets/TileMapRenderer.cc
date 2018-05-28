#include "hvn3/tilesets/TileMapRenderer.h"

namespace hvn3 {

	TileMapRenderer::TileMapRenderer() {}

	void TileMapRenderer::AddTileset(const Tileset& tileset) {
		_tilesets.push_back(tileset);
	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap) {

		if (_tilesets.size() == 0)
			return;

		canvas.HoldBitmapDrawing(true);

		for (auto layer = tilemap.LayersBegin(); layer != tilemap.LayersEnd(); ++layer) {
			for (int i = 0; i < tilemap.Height(); ++i)
				for (int j = 0; j < tilemap.Width(); ++j) {

					TileMap::Tile tile_data = tilemap.At(i, j, layer->first);

					if (tile_data.id <= 0)
						continue;

					const Graphics::Bitmap& tile_image = _tilesets[0].At(tile_data.id - 1).bitmap;
					SizeI tile_size = _tilesets[0].GridSize();

					canvas.DrawBitmap(i * tile_size.width, j * tile_size.height, tile_image);

				}
		}

		canvas.HoldBitmapDrawing(false);

	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y) {



	}
	void TileMapRenderer::Render(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region) {



	}

}