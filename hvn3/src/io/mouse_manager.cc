#include "core/engine.h"
#include "io/mouse_manager.h"

#include <allegro5/allegro.h>

namespace hvn3::io {

	// public members

	MouseManager::MouseManager(events::IEventManager& eventManager) :
		eventManager(eventManager) {

		core::Engine::Initialize(core::EngineModules::IO);

		eventSource = events::EventSource(al_get_mouse_event_source());

		eventManager.GetEventQueue().RegisterEventSource(GetEventSource());

	}
	MouseManager::~MouseManager() {

		core::Engine::Deinitialize(core::EngineModules::IO);

	}

	const events::IEventSource& MouseManager::GetEventSource() const {

		return eventSource;

	}

}