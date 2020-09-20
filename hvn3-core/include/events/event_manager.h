#pragma once

#include "events/event_queue.h"
#include "events/ievent_manager.h"
#include "services/di_service_container.h"

namespace hvn3::events {

	class EventManager :
		public IEventManager {

	public:
		IEventQueue& GetEventQueue() override;
		const IEventQueue& GetEventQueue() const override;
		MultiEventBus& GetEventBus() override;
		const MultiEventBus& GetEventBus() const override;

	private:
		EventQueue eventQueue;
		MultiEventBus eventBus;

	};

}