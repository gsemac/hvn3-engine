#pragma once

#include "events/event_base.h"

namespace hvn3::events {

	class TickEvent :
		EventBase<EventType::Timer> {
	};

}