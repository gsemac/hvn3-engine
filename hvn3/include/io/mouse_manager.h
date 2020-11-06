#pragma once

#include "events/event_source.h"
#include "events/ievent_manager.h"
#include "io/imouse_manager.h"
#include "services/di_service_container.h"

namespace hvn3::io {

	class MouseManager :
		public IMouseManager {

	public:
		HVN3_INJECT(MouseManager(events::IEventManager& eventManager));
		~MouseManager();

		const events::IEventSource& GetEventSource() const override;

	private:
		events::IEventManager& eventManager;
		events::EventSource eventSource;

	};

}