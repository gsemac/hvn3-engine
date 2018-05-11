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
				_tileset(tileset),
				_selection(0, 0, 0, 0) {
				_dragging = false;
			}

			const RectangleI& Selection() {
				return _selection;
			}
			void SetSelection(float x, float y, float w, float h) {
				SetSelection(RectangleI(x, y, w, h));
			}
			void SetSelection(const RectangleI& selection) {
				_selection = selection;
			}
			const Tileset& Tileset() {
				return _tileset;
			}

			void OnMousePressed(WidgetMousePressedEventArgs& e) override {

				PointI tile_index = _mousePositionToTileIndex(e.Position());
				
				if (tile_index.x < 0 && tile_index.y < 0)
					return;

				_selected_index = tile_index;
				_selection.SetPosition(tile_index);
				_selection.SetSize(1, 1);

				_dragging = true;

			}
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override {
				_dragging = false;
			}
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override {

				if (_dragging) {

					PointI tile_index = _mousePositionToTileIndex(e.Position());

					if (tile_index.x < 0)
						tile_index.x = _selected_index.x;
					if (tile_index.y < 0)
						tile_index.y = _selected_index.y;

					_selection = RectangleI::Union(RectangleI(_selected_index, SizeI(1, 1)), RectangleI(tile_index, SizeI(1, 1)));

				}

			}
			void OnDraw(WidgetDrawEventArgs& e) override {

				e.Graphics().DrawSolidRectangle(RectangleF(Position(), Size()), Color(Color::Black, 50));

				e.Graphics().DrawBitmap(X() - VisibleRegion().X(), Y() - VisibleRegion().Y(), _tileset.Bitmap());

				_drawSelectorRectangle(e);

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
			hvn3::Tileset _tileset;
			PointI _selected_index;
			RectangleI _selection;
			bool _dragging;

			void _drawSelectorRectangle(WidgetDrawEventArgs& e) {

				RectangleF selection_rect = _selection;
				PointF offset = Position() - VisibleRegion().Position();
				SizeI size = _tileset.TileSize();

				selection_rect.SetX(_selection.X() * size.width + offset.x);
				selection_rect.SetY(_selection.Y() * size.height + offset.y);
				selection_rect.SetWidth(selection_rect.Width() * size.width);
				selection_rect.SetHeight(selection_rect.Height() * size.height);

				e.Graphics().SetBlendMode(hvn3::Graphics::BlendOperation::Invert);
				e.Graphics().DrawRectangle(selection_rect, Color::White, 3.0f);
				e.Graphics().ResetBlendMode();

			}
			PointI _mousePositionToTileIndex(const PointF& pos) {

				PointI offset = pos - FixedPosition();
				offset += VisibleRegion().Position();

				if (offset.x >= _tileset.Bitmap().Width())
					offset.x = _tileset.Bitmap().Width() - _tileset.TileSize().width;

				if (offset.y >= _tileset.Bitmap().Height())
					offset.y = _tileset.Bitmap().Height() - _tileset.TileSize().height;

				return PointI(offset.x / _tileset.TileSize().width, offset.y / _tileset.TileSize().height);

			}

		};

	}
}