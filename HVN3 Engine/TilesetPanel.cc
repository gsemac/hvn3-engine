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
			_selected_region(0, 0, 0, 0) {

			UpdateScrollableRegion();
			
			_mouse_held = false;

		}

		void TilesetPanel::OnPaint(PaintEventArgs& e) {

			// Draw background color.
			e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());
			
			// Draw tiles.
			e.Graphics().DrawBitmap(-ScrollPosition().X(), -ScrollPosition().Y(), _tileset->Bitmap());

			if (_selected_region.X() >= 0 && _selected_region.Y() >= 0) {
				float x = _selected_region.X();
				float y = _selected_region.Y();
				float ow = 2.0f;
				e.Graphics().SetBlendMode(Drawing::BlendOperation::Invert);
				e.Graphics().DrawRectangle(x - ScrollPosition().X(), y - ScrollPosition().Y(), _selected_region.Width(), _selected_region.Height(), Color::White, ow);
				e.Graphics().ResetBlendMode();
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

			// If the mouse button isn't being held down, do nothing.
			if (!_mouse_held)
				return;

			// If the mouse is not on the bitmap, do nothing.
			if (!PositionIsOnBitmap(e.Position()))
				return;

			// Offset the mouse position by the scroll position.
			PointF mouse_pos = e.Position();
			mouse_pos += ScrollPosition();

			// Convert the mouse position to a grid position.
			PointF tile_pos = SnapToTilesetGrid(mouse_pos);

			// Set the selected region.
			int w = _tileset->TileSize().Width();
			int h = _tileset->TileSize().Height();
			_selected_region = RectangleI::Union(
				RectangleI(_selected_tile.X() * w, _selected_tile.Y() * h, w, h),
				RectangleI(tile_pos.X() * w, tile_pos.Y() * h, w, h)
				);
		
			Invalidate();

			ScrollableControl::OnMouseMove(e);

		}
		void TilesetPanel::OnMouseDown(MouseEventArgs& e) {
	
			// If the click was not on the bitmap, do nothing.
			if (!PositionIsOnBitmap(e.Position()))
				return;
			
			// Offset the mouse position by the scroll position.
			PointF mouse_pos = e.Position();
			mouse_pos += ScrollPosition();

			// Set the clicked tile as the new selected tile.
			_selected_tile = SnapToTilesetGrid(mouse_pos);
			int w = _tileset->TileSize().Width();
			int h = _tileset->TileSize().Height();
			_selected_region = RectangleI(_selected_tile.X() * w, _selected_tile.Y() * h, w, h);

			// The mouse is now being held down on the control.
			_mouse_held = true;

			Invalidate();

			ScrollableControl::OnMouseDown(e);

		}
		void TilesetPanel::OnMouseUp(MouseEventArgs& e) {



		}
		void TilesetPanel::OnUpdate(UpdateEventArgs& e) {

			// If the mouse button is no longer being pressed, acknowledge.
			if (!Mouse::ButtonDown(MouseButton::Left))
				_mouse_held = false;


		}

		PointI TilesetPanel::SelectedTilePosition() const {

			return _selected_tile;

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
		PointI TilesetPanel::SnapToTilesetGrid(const PointF& position) {

			return  PointI(Math::Floor(position.X() / 32), Math::Floor(position.Y() / 32));

		}
		bool TilesetPanel::PositionIsOnBitmap(const PointF& position) {

			return Math::Geometry::PointIn(position, RectangleF(-ScrollPosition().X(), -ScrollPosition().Y(), _tileset->Bitmap().Width(), _tileset->Bitmap().Height()));

		}

	}
}