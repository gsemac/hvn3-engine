#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/tilesets/Tileset.h"
#include <vector>

namespace hvn3 {

	class TileMapRenderer {

	public:
		TileMapRenderer() {}

		void AddTileset(const Tileset& tileset) {
			_tilesets.push_back(tileset);
		}
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap) {

			if (_tilesets.size() == 0)
				return;

			canvas.HoldBitmapDrawing(true);

			for (auto layer = tilemap.LayersBegin(); layer != tilemap.LayersEnd(); ++layer) {
				for (int i = 0; i < tilemap.Height(); ++i)
					for (int j = 0; j < tilemap.Width(); ++j) {

						TileMap::Tile tile_data = tilemap.At(i, j, layer->first);

						if (tile_data.id <= 0)
							continue;

						Graphics::Bitmap tile_image = _tilesets[0].At(tile_data.id - 1);
						SizeI tile_size = _tilesets[0].GridSize();

						canvas.DrawBitmap(i * tile_size.width, j * tile_size.height, tile_image);

					}
			}

			canvas.HoldBitmapDrawing(false);

		}
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap, float x, float y) {



		}
		void Render(Graphics::Graphics& canvas, const TileMap& tilemap, const RectangleF& region) {



		}

	private:
		std::vector<Tileset> _tilesets;

	};

}