#pragma once

#include "../events/allegro_event_source.h"
#include "io/imouse_manager.h"

namespace hvn3::io {

	class AllegroMouseManager :
		public hvn3::io::IMouseManager {

	public:
		AllegroMouseManager();
		~AllegroMouseManager();
		
		events::AllegroEventSource GetEventSource();

	};

}