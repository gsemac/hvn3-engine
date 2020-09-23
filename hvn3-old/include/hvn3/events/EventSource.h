#pragma once

#include "hvn3/allegro/AllegroForwardDeclarations.h"

namespace hvn3 {

	class EventQueue;

	class EventSource {
		friend class EventQueue;

	public:
		EventSource(ALLEGRO_EVENT_SOURCE* source);

	private:
		ALLEGRO_EVENT_SOURCE* _event_source;

	};

}