#pragma once

#include <cstddef>

namespace hvn3::events {

	class IEventBus {

	public:
		typedef size_t size_type;

		virtual ~IEventBus() = default;

		virtual bool Unsubscribe(void* eventListener) = 0;

		// Returns the number of listeners in the container.
		virtual size_type Count() const = 0;

	};

}