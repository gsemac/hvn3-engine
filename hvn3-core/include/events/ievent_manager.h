#pragma once

#include "events/ievent_queue.h"
#include "events/multi_event_bus.h"

namespace hvn3::events {

	class IEventManager {

	public:
		virtual ~IEventManager() = default;

		virtual MultiEventBus& GetEventBus() = 0;
		virtual const MultiEventBus& GetEventBus() const = 0;

	};

}