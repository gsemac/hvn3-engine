#pragma once

#include "events/event_base.h"
#include "events/event_type.h"
#include "io/mouse_button.h"
#include "math/point.h"

namespace hvn3::events {

	template<EventType EVENT_TYPE = EventType::Unspecified>
	class MouseEventBase :
		public EventBase<EVENT_TYPE> {

	public:
		MouseEventBase(const math::Point2i& position, const math::Point2i& wheelOffset, io::MouseButton buttons, int clicks) :
			position(position),
			wheelOffset(wheelOffset),
			buttons(buttons),
			clicks(clicks) {
		}

		int X() const {

			return position.X();

		}
		int Y() const {

			return position.Y();

		}
		const math::Point2i& Position() const {

			return position;

		}
		const math::Point2i& WheelOffset() const {

			return wheelOffset;

		}
		const io::MouseButton& Button() const {

			return buttons;

		}
		int Clicks() const {

			return clicks;

		}

	private:
		math::Point2i position;
		math::Point2i wheelOffset;
		io::MouseButton buttons;
		int clicks;

	};

	class MouseMoveEvent :
		public MouseEventBase<EventType::MouseAxes> {

	public:
		using MouseEventBase::MouseEventBase;

	};

}