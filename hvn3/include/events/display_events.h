#pragma once

#include "core/idisplay.h"
#include "core/type_list.h"
#include "events/event_base.h"

namespace hvn3::events {

	class DisplayCloseEvent :
		public EventBase<EventType::DisplayClose> {
	};

	class DisplayResizeEvent :
		public EventBase<EventType::DisplayResize> {

	public:
		DisplayResizeEvent(core::IDisplay& display);

		core::IDisplay& Display();
		const core::IDisplay& Display() const;

	private:
		core::IDisplay& display;

	};

	using DisplayEvents = core::TypeList<
		DisplayCloseEvent,
		DisplayResizeEvent
	>;

}