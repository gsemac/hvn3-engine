#pragma once

#include "hvn3/graphics/GridRendererExt.h"
#include "hvn3/gui/DragOffset.h"
#include "hvn3/gui/ScrollableWidgetBase.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"

#include <cassert>

namespace hvn3 {
	namespace Gui {

		class RoomView :
			public WidgetBase {

		public:
			RoomView() :
				_grid_cell_size(32.0f, 32.0f),
				_room_scale(1.0f) {

				_show_grid = true;
				_draw_objects = true;
				_draw_outside_room = true;

				_dragging = false;
				_drag_button_held = false;

				_zoom_level = 0;

				_pan_enabled = true;
				_zoom_enabled = true;

				// Enable key preview so we can check for things like click+spacebar.
				SetKeyPreviewEnabled(true);

				Graphics::Pen pen(hvn3::Color(hvn3::Color::Black, 128));
				pen.SetDashPattern({ 2.0f });
				_fg_grid.SetLineStyle(pen);

				_bg_grid.SetLinesVisible(false);
				_bg_grid.SetCellColors({ Color::White, Color::Silver });

			}
			RoomView(const IRoomPtr& room) :
				RoomView() {

				_room = room;

			}

			void SetRoom(const IRoomPtr& room) {

				_room = room;

			}
			void SetGridVisible(bool value) {
				_show_grid = value;
			}
			void SetObjectsVisible(bool value, bool draw_outside_room) {

				_draw_objects = value;
				_draw_outside_room = draw_outside_room;

			}

			PointF DisplayPositionToWorldPosition(const PointF& position, bool snap_to_grid) const {

				assert(static_cast<bool>(_room));

				PointF p = _positionToRoomPosition(position);

				if (snap_to_grid) {
					p.x = Math::Floor(p.x, _grid_cell_size.width);
					p.y = Math::Floor(p.y, _grid_cell_size.height);
				}

				return p;

			}
			PointF DisplayPositionToGridCell(const PointF& position) const {

				PointF p = _positionToRoomPosition(position);
				p.x = Math::Floor(p.x / _grid_cell_size.width);
				p.y = Math::Floor(p.y / _grid_cell_size.height);

				return p;

			}
			PointF WorldPositionToDisplayPosition(const PointF& position) const {

				PointF global_pos = position;

				global_pos *= _room_scale.Inverse();
				global_pos += FixedPosition();

				return global_pos;

			}
			bool IsPositionInRoom(const PointF& position) const {

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

			PointF RoomPosition() const {

				return _drag_offset.Offset();

			}
			void SetRoomPosition(const PointF& position) {

				_drag_offset.SetOffset(position);

			}
			Scale RoomScale() const {
				return _room_scale;
			}
			void SetRoomScale(const Scale& value) {

				_room_scale = value;

			}
			void SetZoom(float factor) {
				SetZoom(PointF(0.0f, 0.0f), factor);
			}
			void SetZoom(const PointF& focus, float factor) {

				float old_offset_x = _drag_offset.X();
				float old_offset_y = _drag_offset.Y();
				float old_factor = _room_scale.Factor();

				// Calculate the position, in the room, that the cursor is hovering over.

				float world_position_x = (focus.x - old_offset_x) / old_factor;
				float world_position_y = (focus.y - old_offset_y) / old_factor;

				// Adjust the room scale the new scale factor.

				SetRoomScale(Scale(factor));

				// Multiply the cursor's room position by the same scale factor.
				// This represents the new position, in the room, that the cursor should be hovering over.

				float new_world_position_x = world_position_x * factor;
				float new_world_position_y = world_position_y * factor;

				// Calculate the offset required so that the cursor is hovering over this new position.

				float offset_x = focus.x - new_world_position_x;
				float offset_y = focus.y - new_world_position_y;

				// Offset the room by this amount.

				_drag_offset.SetOffset(Math::Round(offset_x), Math::Round(offset_y));

			}

			void SetPanEnabled(bool value) {

				_pan_enabled = value;

			}
			void SetZoomEnabled(bool value) {

				_zoom_enabled = value;

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

				if (e.Button() == MouseButton::Left || e.Button() == MouseButton::Middle) {

					_dragging = true;

					if (e.Button() == MouseButton::Middle)
						_drag_button_held = true;

					if (_drag_button_held)
						_drag_offset.SetClickedPosition(e.Position());

				}

			}
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override {

				if (e.Button() == MouseButton::Left)
					_dragging = false;
				else if (e.Button() == MouseButton::Middle)
					_drag_button_held = false;

			}
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override {

				if (_pan_enabled && _dragging && _drag_button_held)
					_drag_offset.SetDraggedPosition(e.Position());

				_last_mouse_position = e.Position();

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

					float xoff = X() + _drag_offset.X();
					float yoff = Y() + _drag_offset.Y();
					SizeF room_size = static_cast<SizeF>(_room->Size() * _room_scale);

					t.Scale(_room_scale);
					t.Translate(xoff, yoff);

					e.Graphics().SetTransform(t);
					e.Graphics().SetClip(RectangleF::Intersection(clip, RectangleF(FixedPosition() + _drag_offset.Offset(), room_size)));

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
			void OnMouseScroll(WidgetMouseScrollEventArgs& e) override {

				if (!_zoom_enabled)
					return;

				// The floating-point zoom factor is calculated on the spot rather than being incremented/decremented.
				// This is to avoid errors resulting from repeated operations (e.g., adding the zoom increment on each scroll).

				const int MAX_ZOOM_LEVEL = 10;
				const float ZOOM_INCREMENT = 0.5f;

				if (e.Direction() == MouseScrollDirection::Up)
					_zoom_level = Math::Min(_zoom_level + 1, MAX_ZOOM_LEVEL);
				else if (e.Direction() == MouseScrollDirection::Down)
					_zoom_level = Math::Max(_zoom_level - 1, -MAX_ZOOM_LEVEL);

				float zoom = 1.0f + (ZOOM_INCREMENT * static_cast<float>(Math::Abs(_zoom_level)));

				if (_zoom_level < 0)
					zoom = 1.0f / zoom;

				SetZoom(_last_mouse_position - FixedPosition(), zoom);

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

			int _zoom_level;
			PointF _last_mouse_position;

			bool _pan_enabled;
			bool _zoom_enabled;

			PointF _positionToRoomPosition(const PointF& position) const {

				PointF room_origin = FixedPosition();

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

		};

	}
}