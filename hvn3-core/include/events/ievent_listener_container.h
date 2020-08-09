#pragma once

#include <cstddef>

namespace hvn3::events {

	class IEventListenerContainer {

	public:
		typedef size_t size_type;

		// Returns the number of listeners in the container.
		virtual size_type Count() const = 0;

	};

}