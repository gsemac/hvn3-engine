#pragma once

#include "io/idisplay.h"
#include "core/type_list.h"
#include "events/event_base.h"

namespace hvn3::events {

	class DisplayCloseEvent :
		public EventBase<EventType::DisplayClose> {
	};

	class DisplayResizeEvent :
		public EventBase<EventType::DisplayResize> {

	public:
		DisplayResizeEvent(io::IDisplay& display);

		io::IDisplay& Display();
		const io::IDisplay& Display() const;

	private:
		io::IDisplay& display;

	};

	using DisplayEvents = core::TypeList<
		DisplayCloseEvent,
		DisplayResizeEvent
	>;

}