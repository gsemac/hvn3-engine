#pragma once

#include "events/ievent_filter.h"
#include "events/ievent_queue.h"
#include "events/ievent_queue.h"
#include "events/multi_event_bus.h"

#include <memory>

namespace hvn3::events {

	class IEventManager {

	protected:
		using EventFilterHandle = std::shared_ptr<IEventFilter>;

	public:
		virtual ~IEventManager() = default;

		virtual IEventQueue& GetEventQueue() = 0;
		virtual const IEventQueue& GetEventQueue() const = 0;
		virtual MultiEventBus& GetEventBus() = 0;
		virtual const MultiEventBus& GetEventBus() const = 0;

		virtual void AddEventFilter(const EventFilterHandle& eventFilter) = 0;

		virtual bool DoEvents(bool waitForEvent = false) = 0;

	};

}