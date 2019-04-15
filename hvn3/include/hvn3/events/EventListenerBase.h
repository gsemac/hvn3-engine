#pragma once

#include <type_traits>

namespace hvn3 {

	namespace impl {

		template<typename EventType>
		class EventListenerBaseMethodHelper {

			typedef typename std::conditional<std::is_fundamental<EventType>::value, EventType, EventType&>::type parameter_type;

		public:
			virtual void OnEvent(parameter_type ev) = 0;

		};

	}

	// Base class for event listeners to be passed into an EventListener object.
	template<typename... EventTypes>
	class EventListenerBase :
		public impl::EventListenerBaseMethodHelper<EventTypes>... {
	};

}