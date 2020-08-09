#pragma once

#include <limits>

namespace hvn3::events {

	enum class EventListenerPriority {
		High = std::numeric_limits<int>::max(),
		Normal = 0,
		Low = std::numeric_limits<int>::min()
	};

}