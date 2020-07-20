#include "events/read_only_event_source.h"

namespace hvn3::events {

	ReadOnlyEventSource::ReadOnlyEventSource() :
		EventSourceBase(nullptr, false) {
	}
	ReadOnlyEventSource::ReadOnlyEventSource(underlying_t* eventSource, bool takeOwnership) :
		EventSourceBase(eventSource, takeOwnership) {
	}

	ReadOnlyEventSource::operator bool() const {

		return EventSourceBase::operator bool();

	}

}