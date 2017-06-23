#include "TilesetPanel.h"
#include "io/Mouse.h"

namespace hvn3 {
	namespace Gui {

		// Public methods

		TilesetPanel::TilesetPanel(const PointF& position, const SizeF& size, const Tileset& tileset) :
			ScrollableControl(SizeF(tileset.Bitmap().Width(), tileset.Bitmap().Height())),
			Control(position, size),
			_tileset(&tileset),
			_hovered_tile_position(-1, -1),
			_selected_tile_region(0, 0) {

			UpdateScrollableRegion();

		}

		void TilesetPanel::OnPaint(PaintEventArgs& e) {

			// Draw background color.
			e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());

			// Draw tiles.
			e.Graphics().DrawBitmap(-ScrollPosition().X(), -ScrollPosition().Y(), _tileset->Bitmap());

			// Draw a box around the tile that the mouse is currently hovering over.
			e.Graphics().DrawFilledRectangle(_hovered_tile_position.X() * 32, _hovered_tile_position.Y() * 32, 32, 32, Color::FromArgbf(1, 1, 1, 0.2));
			// #todo draw a grid over the tiles

			e.Graphics().SetBlendMode(Drawing::BlendOperation::Invert);
			e.Graphics().DrawFilledRectangle(0, 0, 64, 64, Color::White);
			e.Graphics().ResetBlendMode();
			
			if (_selected_tile_region.X() >= 0 && _selected_tile_region.Y() >= 0) {
				float x = _selected_tile_region.X() * _tileset->TileSize().Width();
				float y = _selected_tile_region.Y() * _tileset->TileSize().Height();
				e.Graphics().DrawRoundRectangle(x - 1, y - 1, _tileset->TileSize().Width() + 2, _tileset->TileSize().Height() + 2, Color::Black, 3, 1);
				e.Graphics().DrawRoundRectangle(x, y, _tileset->TileSize().Width(), _tileset->TileSize().Height(), Color::White, 3, 1);
			}
			
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
		void TilesetPanel::OnMouseMove(MouseMoveEventArgs& e) {

			_hovered_tile_position = SnapToTilesetGrid(e.Position());

			std::cout << e.Position() << std::endl;
			Invalidate();

			ScrollableControl::OnMouseMove(e);

		}
		void TilesetPanel::OnMouseDown(MouseEventArgs& e) {

			_selected_tile_region.SetPosition(SnapToTilesetGrid(e.Position()));

			Invalidate();

			ScrollableControl::OnMouseDown(e);

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
		PointF TilesetPanel::SnapToTilesetGrid(const PointF& position) {

			return  PointF(Floor(position.X() / 32), Floor(position.Y() / 32));

		}

	}
}