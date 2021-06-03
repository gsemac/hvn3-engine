#include "events/event_queue_base.h"

namespace hvn3::events {

	// Public members

	EventQueueBase::operator bool() const {

		return static_cast<bool>(GetHandle());

	}

}