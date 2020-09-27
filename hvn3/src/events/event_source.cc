#include "events/event_source.h"

namespace hvn3::events {

	// Public members

	EventSource::EventSource() :
		EventSource(nullptr) {
	}
	EventSource::EventSource(underlying_t* eventSource) :
		eventSource(std::shared_ptr<underlying_t>{}, eventSource) {
	}
	EventSource::EventSource(std::shared_ptr<underlying_t> eventSource) :
		eventSource(eventSource) {
	}

	EventSource::underlying_t* EventSource::GetUnderlyingData() const {

		return eventSource.get();

	}

	EventSource ::operator bool() const {

		return static_cast<bool>(eventSource);

	}

}