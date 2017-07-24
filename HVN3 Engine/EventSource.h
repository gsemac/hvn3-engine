#pragma once
#include "AllegroForwardDeclarations.h"

namespace hvn3 {
	namespace System {

		class EventQueue;

		class EventSource {
			friend class EventQueue;

		public:
			EventSource(ALLEGRO_EVENT_SOURCE* source);

		private:
			ALLEGRO_EVENT_SOURCE* _event_source;

		};

	}
}