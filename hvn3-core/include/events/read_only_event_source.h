#pragma once

#include "events/event_source_base.h"

namespace hvn3::events {

	class ReadOnlyEventSource :
		public EventSourceBase {

		using underlying_t = ALLEGRO_EVENT_SOURCE;

	public:
		ReadOnlyEventSource();
		ReadOnlyEventSource(underlying_t* eventSource, bool takeOwnership);

		explicit operator bool() const;

	};

}