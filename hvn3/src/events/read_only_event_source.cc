#include "events/read_only_event_source.h"

namespace hvn3::events {

	// Public members

	ReadOnlyEventSource::ReadOnlyEventSource() :
		ReadOnlyEventSource(nullptr) {
	}
	ReadOnlyEventSource::ReadOnlyEventSource(underlying_t* eventSource) {

		this->eventSource = eventSource;

	}

	ReadOnlyEventSource::underlying_t* ReadOnlyEventSource::GetUnderlyingData() const {

		return eventSource;

	}

	ReadOnlyEventSource::operator bool() const {

		return eventSource != nullptr;

	}

}