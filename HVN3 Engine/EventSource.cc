#include <allegro5/allegro.h>
#include "EventSource.h"

namespace hvn3 {

	EventSource::EventSource(ALLEGRO_EVENT_SOURCE* source) {

		__ev_source = source;

	}

}