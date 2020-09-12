#pragma once

#include "events/ievent_manager.h"
#include "services/di_service_container.h"

namespace hvn3::events {

	class EventManager :
		public IEventManager {

	public:
		MultiEventBus& GetEventBus() override;
		const MultiEventBus& GetEventBus() const override;

	private:
		MultiEventBus eventBus;

	};

}