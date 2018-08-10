#pragma once
#include "hvn3/graphics/GridRendererExt.h"
#include "hvn3/gui2/ScrollableWidgetBase.h"
#include "hvn3/rooms/IRoom.h"

namespace hvn3 {
	namespace Gui {

		class RoomView :
			public ScrollableWidgetBase {

		public:
			RoomView(const RoomPtr& room) :
				ScrollableWidgetBase(SizeF(room->Width(), room->Height())),
				_room(room) {

				_show_grid = true;

				Graphics::Pen pen(hvn3::Color(hvn3::Color::Black, 128));
				pen.SetDashPattern({ 2.0f });
				_grid_renderer.SetLineStyle(pen);

			}

			void SetRoom(const RoomPtr& room) {
				
				_room = room;

				SetScrollableSize(static_cast<SizeF>(_room->Size()));

			}
			void SetGridVisible(bool value) {
				_show_grid = value;
			}
			PointF PositionToRoomPosition(const PointF& position) const {
				
				PointF null(-1.0f, -1.0f);
				PointF out = _positionToRoomPosition(position);
				
				if (!_room)
					return null;

				if (out.x < 0.0f || out.y < 0.0f || out.x > _room->Width() || out.y > _room->Height())
					return null;

				return out;

			}
			PointI PositionToGridPosition(const PointF& position) const {

				PointF null(-1.0f, -1.0f);
				PointF out = _positionToRoomPosition(position);

				if (!_room)
					return null;

				if (out.x < 0.0f || out.y < 0.0f || out.x > Math::Ceiling(_room->Width(), 32) || out.y > Math::Ceiling(_room->Height(), 32))
					return null;

				out.x = Math::Floor(out.x / 32.0f);
				out.y = Math::Floor(out.y / 32.0f);

				return static_cast<PointI>(out);

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

					e.Graphics().SetClip(clip);

					if (_show_grid) {

						Grid grid(static_cast<hvn3::SizeF>(_room->Size()), hvn3::SizeF(32.0f, 32.0f), true);
						_grid_renderer.Draw(e.Graphics(), PointF(0.0f, 0.0f), grid);

					}

					e.Graphics().Restore(original_graphics_state);

				}

			}

		private:
			RoomPtr _room;
			bool _show_grid;
			hvn3::Graphics::GridRendererExt _grid_renderer;

			PointF _positionToRoomPosition(const PointF& position) const {

				PointF room_origin = FixedPosition();
				room_origin -= VisibleRegion().Position();

				return position - room_origin;

			}

		};

	}
}