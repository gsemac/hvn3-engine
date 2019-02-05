#pragma once

#include "hvn3/events/EventSource.h"

namespace hvn3 {

	class IEventArgs {

	public:
		virtual ~IEventArgs() = default;

		// Returns the event source that the event originated from.
		virtual const EventSource& Source() const = 0;
		// Returns the time the event was emitted.
		virtual double Timestamp() const = 0;
		// Returns true if the event has been handled by another event handler; returns false otherwise.
		virtual bool Handled() const = 0;
		// Flags the event as having already been handled.
		virtual void SetHandled(bool value) = 0;

	};

}