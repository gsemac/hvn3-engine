#include "events/timer_events.h"

namespace hvn3::events {

	TickEvent::TickEvent() :
		EventBase(EventType::Timer) {
	}

}