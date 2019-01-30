#pragma once

#include "hvn3/graphics/GridRendererExt.h"
#include "hvn3/gui2/DragOffset.h"
#include "hvn3/gui2/ScrollableWidgetBase.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"

#include <cassert>

namespace hvn3 {
	namespace Gui {

		class RoomView :
			public ScrollableWidgetBase {

		public:
			RoomView() :
				ScrollableWidgetBase(SizeF(0.0f, 0.0f)),
				_grid_cell_size(32.0f, 32.0f),
				_room_scale(1.0f) {

				_show_grid = true;
				_draw_objects = true;
				_draw_outside_room = true;

				_dragging = false;
				_drag_button_held = false;

				Graphics::Pen pen(hvn3::Color(hvn3::Color::Black, 128));
				pen.SetDashPattern({ 2.0f });
				_fg_grid.SetLineStyle(pen);

				_bg_grid.SetLinesVisible(false);
				_bg_grid.SetCellColors({ Color::White, Color::Silver });

			}
			RoomView(const IRoomPtr& room) :
				RoomView() {

				_room = room;

				if (_room)
					SetScrollableSize(SizeF(_room->Width(), _room->Height()));

			}

			void SetRoom(const IRoomPtr& room) {

				_room = room;

				_updateScrollableRegion();

			}
			void SetGridVisible(bool value) {
				_show_grid = value;
			}
			void SetObjectsVisible(bool value, bool draw_outside_room) {

				_draw_objects = value;
				_draw_outside_room = draw_outside_room;

			}

			PointF GlobalPositionToRoomPosition(const PointF& position, bool snap_to_grid) const {

				assert(static_cast<bool>(_room));

				PointF p = _positionToRoomPosition(position);

				if (snap_to_grid) {
					p.x = Math::Floor(p.x, _grid_cell_size.width);
					p.y = Math::Floor(p.y, _grid_cell_size.height);
				}

				return p;

			}
			PointF GlobalPositionToGridCell(const PointF& position) const {

				PointF p = _positionToRoomPosition(position);
				p.x = Math::Floor(p.x / _grid_cell_size.width);
				p.y = Math::Floor(p.y / _grid_cell_size.height);

				return p;

			}
			PointF RoomPositionToGlobalPosition(const PointF& position) const {

				PointF global_pos = position;

				global_pos *= _room_scale.Inverse();
				global_pos += FixedPosition() - VisibleRegion().Position();

				return global_pos;

			}
			bool IsPositionInRoomBounds(const PointF& position) const {

				if (position.x < 0.0f || position.y < 0.0f || position.x > _room->Width() || position.y > _room->Height())
					return false;

				return true;

			}

			void SetGridCellSize(const SizeF& size) {
				_grid_cell_size = size;
			}
			const SizeF& GridCellSize() const {
				return _grid_cell_size;
			}
			Scale RoomScale() const {
				return _room_scale;
			}
			void SetRoomScale(const Scale& value) {

				_room_scale = value;

				_updateScrollableRegion();

			}

			void OnKeyPressed(WidgetKeyPressedEventArgs& e) override {

				if (e.Key() == Key::Space) {

					_drag_button_held = true;

					if (_dragging)
						_drag_offset.SetClickedPosition(Mouse::Position());

				}

			}
			void OnKeyUp(WidgetKeyUpEventArgs& e) override {

				if (e.Key() == Key::Space)
					_drag_button_held = false;

			}
			void OnMousePressed(WidgetMousePressedEventArgs& e) override {

				if (e.Button() == MouseButton::Left) {

					_dragging = true;

					if (_drag_button_held)
						_drag_offset.SetClickedPosition(e.Position());

				}

			}
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override {

				if (e.Button() == MouseButton::Left)
					_dragging = false;

			}
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override {

				if (_dragging && _drag_button_held) {

					_drag_offset.SetDraggedPosition(e.Position());



				}

			}
			void OnDraw(WidgetDrawEventArgs& e) override {

				// Draw background grid.

				Grid grid(Size(), SizeF(16.0f, 16.0f), true);
				_bg_grid.Draw(e.Graphics(), Position(), grid);
				
				if (_room) {

					DrawEventArgs args(e.Graphics());

					Graphics::GraphicsState original_graphics_state = e.Graphics().Save();

					Graphics::Transform t = e.Graphics().GetTransform();
					RectangleF clip = e.Graphics().Clip();

					float xoff = X() - VisibleRegion().X() + _drag_offset.X();
					float yoff = Y() - VisibleRegion().Y() + _drag_offset.Y();
					SizeF room_size = static_cast<SizeF>(_room->Size() * _room_scale);

					t.Scale(_room_scale);
					t.Translate(xoff, yoff);

					e.Graphics().SetTransform(t);
					e.Graphics().SetClip(RectangleF::Intersection(clip, RectangleF(FixedPosition() - VisibleRegion().Position() + _drag_offset.Offset(), room_size)));

					_room->OnDraw(args);

					if (_draw_objects && !_draw_outside_room)
						_drawObjects(e);

					e.Graphics().SetClip(clip);

					if (_draw_objects && _draw_outside_room)
						_drawObjects(e);

					e.Graphics().Restore(original_graphics_state);

					if (_show_grid) {

						SizeF cell_size = _grid_cell_size * _room_scale;
						Grid grid(room_size, cell_size, true);

						_fg_grid.Draw(e.Graphics(), PointF(xoff, yoff), grid);

					}

				}

			}

		private:
			IRoomPtr _room;
			bool _show_grid;
			bool _draw_objects;
			bool _draw_outside_room;
			SizeF _grid_cell_size;
			Scale _room_scale;
			hvn3::Graphics::GridRendererExt _fg_grid;
			hvn3::Graphics::GridRendererExt _bg_grid;

			bool _dragging;
			bool _drag_button_held;
			gui::DragOffset _drag_offset;

			PointF _positionToRoomPosition(const PointF& position) const {

				PointF room_origin = FixedPosition();
				room_origin -= VisibleRegion().Position();

				PointF pos = position - room_origin;
				pos *= _room_scale.Inverse();

				return pos;

			}
			void _drawObjects(WidgetDrawEventArgs& e) {

				DrawEventArgs args(e.Graphics());

				_room->Objects().ForEach([&](IObjectPtr& x) {
					x->OnDraw(args);
				});

			}
			void _updateScrollableRegion() {

				if (_room)
					SetScrollableSize(static_cast<SizeF>(_room->Size() * _room_scale));
				else
					SetScrollableSize(SizeF(0.0f, 0.0f));

			}

		};

	}
}