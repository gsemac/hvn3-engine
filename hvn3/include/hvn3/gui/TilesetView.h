#pragma once
#include "hvn3/graphics/GridRenderer.h"
#include "hvn3/gui/IWidgetRenderer.h"
#include "hvn3/gui/ScrollableWidgetBase.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/tilesets/Tileset.h"

namespace hvn3 {
	namespace Gui {

		class TilesetView :
			public ScrollableWidgetBase {

		public:
			enum class DrawingMode {
				Fixed,
				Collapse
			};

			enum class ViewMode {
				Selection,
				Flags
			};

			TilesetView(const Tileset& tileset) :
				ScrollableWidgetBase(SizeF(tileset.Bitmap().Width(), tileset.Bitmap().Height())),
				_tileset(tileset),
				_selection(0, 0, 1, 1) {

				_dragging = false;
				_view_mode = ViewMode::Selection;

			}

			const RectangleI& SelectedRegion() {
				return _selection;
			}
			void SetSelectedRegion(int x, int y, int w, int h) {
				SetSelectedRegion(RectangleI(x, y, w, h));
			}
			void SetSelectedRegion(const RectangleI& selection) {
				_selection = selection;
			}
			const Tileset& Tileset() {
				return _tileset;
			}
			void SetViewMode(ViewMode value) {
				_view_mode = value;
			}
			ViewMode ViewMode() const {
				return _view_mode;
			}

			void OnMousePressed(WidgetMousePressedEventArgs& e) override {

				PointI tile_index = _mousePositionToTileIndex(e.Position());

				if (tile_index.x < 0 && tile_index.y < 0)
					return;

				if (_view_mode == ViewMode::Selection) {

					_selected_index = tile_index;
					_selection.SetPosition(tile_index);
					_selection.SetSize(1, 1);

					_dragging = true;

				}
				else if (_view_mode == ViewMode::Flags) {

					_tileset.GetAt(tile_index.x, tile_index.y).flag += (e.Button() == MouseButton::Left ? 1 : -1);

				}

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

				if (_view_mode == ViewMode::Flags) {

					e.Graphics().SetBlendMode(hvn3::Graphics::BlendOperation::Invert);

					Graphics::GridRenderer(e.Graphics()).Draw({ X() - VisibleRegion().X(), Y() - VisibleRegion().Y() }, Grid(_tileset.Rows(), _tileset.Columns(), _tileset.TileSize().width, _tileset.TileSize().height), Color::White);

					e.Graphics().SetBitmapDrawingHeld(true);

					for (unsigned int y = 0; y < _tileset.Rows(); ++y)
						for (unsigned int x = 0; x < _tileset.Columns(); ++x) {

							float tx = (X() - VisibleRegion().X()) + x * _tileset.TileSize().width + (_tileset.TileSize().width / 2.0f);
							float ty = (Y() - VisibleRegion().Y()) + y * _tileset.TileSize().height + (_tileset.TileSize().height / 2.0f);

							//e.Graphics().DrawText(tx + 1.0f, ty + 1.0f, StringUtils::ToString(_tileset.At(x, y).flag), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Center | Alignment::Middle);
							e.Graphics().DrawText(tx, ty, StringUtils::ToString(_tileset.At(x, y).flag), GetRenderer()->GetWidgetFont(this), Color::White, Alignment::Center | Alignment::Middle);

						}

					e.Graphics().SetBitmapDrawingHeld(false);
					e.Graphics().ResetBlendMode();

				}
				else if (_view_mode == ViewMode::Selection)
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
			enum class ViewMode _view_mode;

			void _drawSelectorRectangle(WidgetDrawEventArgs& e) {

				RectangleF selection_rect = _selection;
				PointF offset = Position() - VisibleRegion().Position();
				SizeI size = _tileset.GridSize();

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

				//if (offset.x >= _tileset.Bitmap().Width())
				//	offset.x = _tileset.Bitmap().Width() - _tileset.GridSize().width  + 1;

				//if (offset.y >= _tileset.Bitmap().Height())
				//	offset.y = _tileset.Bitmap().Height() - _tileset.GridSize().height + 1;

				PointI index(Math::Min(offset.x / _tileset.GridSize().width, static_cast<int>(_tileset.Columns()) - 1),
					Math::Min(offset.y / _tileset.GridSize().height, static_cast<int>(_tileset.Rows()) - 1));

				return index;

			}

		};

	}
}