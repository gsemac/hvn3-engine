#include "hvn3/exceptions/Exception.h"
#include "hvn3/tiles/AutoTileRenderer.h"
#include "hvn3/tiles/tile_map_renderer.h"

namespace hvn3::tiles {

	// Public members

	TileMapRenderer::TileMapRenderer() {
	}

	void TileMapRenderer::AddTileset(const Tileset& tileset) {

		tilesets.push_back(tileset);

	}

	void TileMapRenderer::DrawTiles(Graphics::Graphics& canvas, const TileMap& tileMap, const SizeI& tileSize) {

		DrawTiles(canvas, tileMap, tileSize, 0, 0);

	}
	void TileMapRenderer::DrawTiles(Graphics::Graphics& canvas, const TileMap& tileMap, const SizeI& tileSize, float x, float y) {

		// If there are no tilesets to use, do nothing.

		if (tilesets.empty())
			return;

		canvas.SuspendBitmapDrawing();

		for (const auto& layer : tileMap.Layers()) {

			for (int y = 0; y < layer.Rows(); ++y) {

				for (int x = 0; x < layer.Columns(); ++x) {

					TileMap::Tile tile = layer.GetTile(x, y);

					if (tile.id <= 0)
						continue;

					TileMap::Tile::tile_id autoTileIndex = tile.AutoTileIndex();

					if (autoTileIndex <= 0) {

						// If the tile is not an autotile, draw it normally.

						const Graphics::Bitmap& image = tilesets[0].GetTile(tile.TileIndex() - 1).Image();

						canvas.DrawBitmap(
							static_cast<float>(x * tileSize.width),
							static_cast<float>(y * tileSize.height),
							image
						);

					}
					else {

						// If the tile is an autotile, we need to draw it according to its autotile index.
						//_auto_tile_renderer.DrawAutoTile(canvas, tile_data, _tilesets[0], x * tile_size.width, y * tile_size.height);

					}

				}

			}

		}

		canvas.ResumeBitmapDrawing();

	}
	void TileMapRenderer::DrawTiles(Graphics::Graphics& canvas, const TileMap& tileMap, const SizeI& tileSize, const RectangleF& region) {

		throw NotImplementedException();

	}

}