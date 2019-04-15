#pragma once

#include "hvn3/events/IUserEvent.h"

namespace hvn3 {

	class IEventListenerContainer {

	public:
		// Dispatches the given event to all listeners. Returns true if the listeners are capable of handling this event type, false otherwise.
		virtual bool Dispatch(IUserEvent& event) = 0;

	};

}