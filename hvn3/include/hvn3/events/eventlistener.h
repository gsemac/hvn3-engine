#pragma once

#include "hvn3/events/EventListenerBase.h"
#include "hvn3/utility/TypeTraits.h"

#include <functional>
#include <tuple>
#include <type_traits>

namespace hvn3 {

	template<typename... EventTypes>
	class EventListener {

	public:
		template<typename EventType>
		using callback_type = std::function<void(typename std::conditional<std::is_fundamental<EventType>::value, EventType, EventType&>::type)>;
		typedef EventListenerBase<EventTypes...> listener_type;

		EventListener();
		EventListener(listener_type* listener);

		// Sets the function called when the given event is received.
		template<typename EventType, typename LambdaType>
		void OnEvent(LambdaType callback) {

			static_assert(is_any<EventType, EventTypes...>::value, "The event listener does not handle this event type");

			_getHandler<EventType>() = std::move(callback);

		}

		// Calls the event handler for the given event type.
		template<typename EventType>
		void OnEvent(EventType& ev) {

			static_assert(is_any<EventType, EventTypes...>::value, "The event listener does not handle this event type");

			if (_listener_object != nullptr)
				static_cast<impl::EventListenerBaseMethodHelper<EventType>*>(_listener_object)->OnEvent(ev);

			auto& callback = _getHandler<EventType>();

			if (callback)
				callback(ev);

		}

	private:
		listener_type* _listener_object;
		std::tuple<callback_type<EventTypes>...> _callbacks;

		template<typename EventType>
		callback_type<EventType>& _getHandler() {
			return std::get<callback_type<EventType>>(_callbacks);
		}

	};

	// Public methods

	template<typename... EventTypes>
	EventListener<EventTypes...>::EventListener() :
		EventListener(nullptr) {}
	template<typename... EventTypes>
	EventListener<EventTypes...>::EventListener(listener_type* listener) :
		_listener_object(listener) {}

}