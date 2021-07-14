#include "events/display_events.h"

namespace hvn3::events {

	DisplayResizeEvent::DisplayResizeEvent(io::IDisplay& display) :
		display(display) {
	}

	io::IDisplay& DisplayResizeEvent::Display() {

		return display;

	}
	const io::IDisplay& DisplayResizeEvent::Display() const {

		return display;

	}

}