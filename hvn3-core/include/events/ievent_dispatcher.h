#pragma once

#include "events/event_listener_base_base.h"

#include <cstddef>

namespace hvn3::events {

	class IEventDispatcher {

	public:
		typedef size_t size_type;

		virtual ~IEventDispatcher() = default;

		virtual bool Unsubscribe(void* eventListener) = 0;

		// Returns the number of listeners in the container.
		virtual size_type Count() const = 0;

	};

}