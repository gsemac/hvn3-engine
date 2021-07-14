#pragma once

#include "core/engine.h"
#include "io/allegro_mouse_manager.h"

#include <allegro5/allegro.h>

namespace hvn3::io {

	// public members

	AllegroMouseManager::AllegroMouseManager() {

		core::Engine::Initialize(core::EngineModules::IO);

	}
	AllegroMouseManager::~AllegroMouseManager() {

		core::Engine::Deinitialize(core::EngineModules::IO);

	}

	events::AllegroEventSource GetEventSource() {

		return events::AllegroEventSource(al_get_mouse_event_source(), false);

	}

}