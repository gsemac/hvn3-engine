#include "TilesetPanel.h"

namespace hvn3 {
	namespace Gui {

		// Public methods

		TilesetPanel::TilesetPanel(const PointF& position, const SizeF& size, const Tileset& tileset) :
			ScrollableControl(SizeF(tileset.Bitmap().Width(), tileset.Bitmap().Height())),
			Control(position, size),
			_tileset(&tileset) {

			UpdateScrollableRegion();

		}

		void TilesetPanel::OnPaint(PaintEventArgs& e) {

			// Draw background color.
			e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());

			// Draw tiles.
			e.Graphics().DrawBitmap(-ScrollPosition().X(), -ScrollPosition().Y(), _tileset->Bitmap());

			// Call the paint event for the underlying control.
			ScrollableControl::OnPaint(e);

			/*
			Tiles used to be drawn separately and split into rows accoriding to the size of the panel.
			The problem with this is that it messed up the alignment with tilesets that care about that.
			*/

			//e.Graphics().HoldBitmapDrawing(true);
			//int x = 0;
			//int y = 0;
			//for (size_t i = 0; i < _tileset->TileCount(); ++i) {
			//	e.Graphics().DrawBitmap(x, y, _tileset->TileAt(i));
			//	x += _tileset->TileSize().Width();
			//	if (x >= Width()) {
			//		x = 0;
			//		y += _tileset->TileSize().Height();
			//	}
			//}
			//e.Graphics().HoldBitmapDrawing(false);

		}

		// Private methods

		void TilesetPanel::UpdateScrollableRegion() {

			//// Calculate the number of tile columns we can show at given width.
			//int cols = static_cast<int>(Floor(Width() / _tileset->TileSize().Width()));

			//// Calculate the number of rows.
			//int rows = static_cast<int>(Ceiling(_tileset->TileCount() / static_cast<float>(cols)));

			//// Set the size of the scrollable region accordingly.
			//SetScrollableRegion(SizeF(Width(), rows * _tileset->TileSize().Height()));

			//// Set visible rows and columns.
			//_rows_and_columns = SizeI(rows, cols);

		}

	}
}