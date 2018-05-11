#pragma once
#include "hvn3/gui2/ScrollableWidgetBase.h"
#include "hvn3/tilesets/Tileset.h"

namespace hvn3 {
	namespace Gui {

		enum class TilesetViewMode {
			Fixed,
			Collapse
		};

		class TilesetView : public ScrollableWidgetBase {

		public:
			TilesetView(const Tileset& tileset) :
				ScrollableWidgetBase(SizeF(tileset.Bitmap().Width(), tileset.Bitmap().Height())),
				_tileset(tileset) {
				SetSmoothScrollEnabled(true);
			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				e.Graphics().DrawSolidRectangle(RectangleF(Position(), Size()), Color(Color::Black, 50));

				e.Graphics().DrawBitmap(X() - VisibleRegion().X(), Y() - VisibleRegion().Y(), _tileset.Bitmap());

				//e.Graphics().HoldBitmapDrawing(true);

				/*float tile_drawing_x = 0.0f;
				float tile_drawing_y = 0.0f;

				for (size_t i = 0; i < _tileset.TileCount(); ++i) {

					e.Graphics().DrawBitmap(X() + tile_drawing_x, Y() + tile_drawing_y, _tileset.TileAt(i));

					tile_drawing_x += _tileset.TileSize().width;
					if (tile_drawing_x + _tileset.TileSize().width > Width()) {
						tile_drawing_x = 0.0f;
						tile_drawing_y += _tileset.TileSize().height;
					}

				}*/

				//e.Graphics().HoldBitmapDrawing(false);

			}

		private:
			Tileset _tileset;

		};

	}
}