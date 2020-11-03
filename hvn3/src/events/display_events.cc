#include "events/display_events.h"

namespace hvn3::events {

	DisplayResizeEvent::DisplayResizeEvent(core::IDisplay& display) :
		display(display) {
	}

	core::IDisplay& DisplayResizeEvent::Display() {

		return display;

	}
	const core::IDisplay& DisplayResizeEvent::Display() const {

		return display;

	}

}