#include <allegro5/allegro.h>
#include "hvn3/events/EventSource.h"

namespace hvn3 {
	namespace System {

		EventSource::EventSource(ALLEGRO_EVENT_SOURCE* source) {

			_event_source = source;

		}

	}
}