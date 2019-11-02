#include "hvn3/exceptions/Exception.h"
#include "hvn3/tiles/AutoTileRenderer.h"
#include "hvn3/tiles/TileMapRenderer.h"

namespace hvn3 {

	TileMapRenderer::TileMapRenderer(const SizeI& tile_size) :
		_tile_size(tile_size) {}

	void TileMapRenderer::AddTileset(const Tileset& tileset) {
		_tilesets.push_back(tileset);
	}
	void TileMapRenderer::Draw(Graphics::Graphics& canvas, const TileMap& tilemap) {

		// If there are no tilesets to use, do nothing.
		if (_tilesets.size() == 0)
			return;

		canvas.SetBitmapDrawingHeld(true);

		for (auto layer = tilemap.LayersBegin(); layer != tilemap.LayersEnd(); ++layer) {
			for (int y = 0; y < tilemap.Rows(); ++y)
				for (int x = 0; x < tilemap.Columns(); ++x) {

					TileMap::Tile tile_data = tilemap.At(x, y, layer->first);

					if (tile_data.id <= 0)
						continue;

					SizeI tile_size = _tile_size;
					TileMap::tile_id auto_tile_index = tile_data.AutoTileIndex();

					if (auto_tile_index <= 0) {

						// If the tile is not an autotile, draw it normally.
						const Graphics::Bitmap& tile_image = _tilesets[0].At(tile_data.TileIndex() - 1).bitmap;
						canvas.DrawBitmap(static_cast<float>(x * tile_size.width), static_cast<float>(y * tile_size.height), tile_image);

					}
					else {

						// If the tile is an autotile, we need to draw it according to its autotile index.
						_auto_tile_renderer.DrawAutoTile(canvas, tile_data, _tilesets[0], x * tile_size.width, y * tile_size.height);

					}

				}
		}

		canvas.SetBitmapDrawingHeld(false);

	}
	void TileMapRenderer::Draw(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y) {

		throw NotImplementedException();

	}
	void TileMapRenderer::Draw(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region) {

		throw NotImplementedException();

	}
	const SizeI& TileMapRenderer::TileSize() const {
		return _tile_size;
	}
	void TileMapRenderer::SetTileSize(const SizeI& value) {
		_tile_size = value;
	}
	const std::vector<Tileset>& TileMapRenderer::Tilesets() const {
		return _tilesets;
	}

}