#include "events/display_events.h"

namespace hvn3::events {

	DisplayCloseEvent::DisplayCloseEvent() :
		EventBase(EventType::DisplayClose) {
	}

}