#include "events/event_manager.h"

namespace hvn3::events {

	// Public members

	IEventQueue& EventManager::GetEventQueue() {

		return eventQueue;

	}
	const IEventQueue& EventManager::GetEventQueue() const {

		return eventQueue;

	}
	MultiEventBus& EventManager::GetEventBus() {

		return eventBus;

	}
	const MultiEventBus& EventManager::GetEventBus() const {

		return eventBus;

	}

}