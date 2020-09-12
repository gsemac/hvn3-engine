#include "events/event_manager.h"

namespace hvn3::events {

	// Public members

	MultiEventBus& EventManager::GetEventBus() {

		return eventBus;

	}
	const MultiEventBus& EventManager::GetEventBus() const {

		return eventBus;

	}

}