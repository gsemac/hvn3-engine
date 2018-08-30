#pragma once
#include "hvn3/graphics/GridRendererExt.h"
#include "hvn3/gui2/ScrollableWidgetBase.h"
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
				_grid_cell_size(32.0f, 32.0f) {

				_show_grid = true;
				_draw_objects = true;
				_draw_outside_room = true;

				Graphics::Pen pen(hvn3::Color(hvn3::Color::Black, 128));
				pen.SetDashPattern({ 2.0f });
				_grid_renderer.SetLineStyle(pen);

			}
			RoomView(const RoomPtr& room) : 
				RoomView() {

				_room = room;

				if (_room)
					SetScrollableSize(SizeF(_room->Width(), _room->Height()));
							   
			}

			void SetRoom(const RoomPtr& room) {

				_room = room;

				SetScrollableSize(static_cast<SizeF>(_room->Size()));

			}
			void SetGridVisible(bool value) {
				_show_grid = value;
			}
			void SetObjectsVisible(bool value, bool draw_outside_room) {

				_draw_objects = value;
				_draw_outside_room = draw_outside_room;

			}

			PointF PositionToRoomPosition(const PointF& position, bool snap_to_grid) const {

				assert(static_cast<bool>(_room));

				PointF p = _positionToRoomPosition(position);

				if (snap_to_grid) {
					p.x = Math::Floor(p.x, _grid_cell_size.width);
					p.y = Math::Floor(p.y, _grid_cell_size.height);
				}

				return p;

			}
			PointF PositionToGridCell(const PointF& position) const {

				PointF p = _positionToRoomPosition(position);

				p.x = Math::Floor(p.x / _grid_cell_size.width);
				p.y = Math::Floor(p.y / _grid_cell_size.height);

				return p;

			}
			bool IsPositionInRoomBounds(const PointF& position) const {

				if (position.x < 0.0f || position.y < 0.0f || position.x > _room->Width() || position.y > _room->Height())
					return false;

				return true;

			}
			void SetGridCellSize(const SizeF& size) {
				_grid_cell_size = size;
			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				if (_room) {

					Graphics::GraphicsState original_graphics_state = e.Graphics().Save();
					RectangleF clip = e.Graphics().Clip();

					Graphics::Transform t = e.Graphics().GetTransform();
					t.Translate(X() - VisibleRegion().X(), Y() - VisibleRegion().Y());

					e.Graphics().SetTransform(t);
					e.Graphics().SetClip(RectangleF::Intersection(clip, RectangleF(FixedPosition(), static_cast<SizeF>(_room->Size()))));

					_room->OnDraw(DrawEventArgs(e.Graphics()));

					if (_draw_objects && !_draw_outside_room)
						_drawObjects(e);

					e.Graphics().SetClip(clip);

					if (_draw_objects && _draw_outside_room)
						_drawObjects(e);

					if (_show_grid) {

						Grid grid(static_cast<SizeF>(_room->Size()), _grid_cell_size, true);
						_grid_renderer.Draw(e.Graphics(), PointF(0.0f, 0.0f), grid);

					}

					e.Graphics().Restore(original_graphics_state);

				}

			}

		private:
			RoomPtr _room;
			bool _show_grid;
			bool _draw_objects;
			bool _draw_outside_room;
			SizeF _grid_cell_size;
			hvn3::Graphics::GridRendererExt _grid_renderer;

			PointF _positionToRoomPosition(const PointF& position) const {

				PointF room_origin = FixedPosition();
				room_origin -= VisibleRegion().Position();

				return position - room_origin;

			}
			void _drawObjects(WidgetDrawEventArgs& e) {

				_room->GetObjects().ForEach([&](IObject* x) {
					x->OnDraw(DrawEventArgs(e.Graphics()));
				});

			}

		};

	}
}