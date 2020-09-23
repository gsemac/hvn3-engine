#include "events/event_base.h"

namespace hvn3::events {

	// Public members

	EventType EventBase::Type() const {

		return eventType;

	}
	double EventBase::Timestamp() const {

		return timestamp;

	}

	// Protected members

	EventBase::EventBase() :
		EventBase(EventType::Unspecified) {
	}
	EventBase::EventBase(EventType eventType) :
		eventType(eventType) {
	}

}