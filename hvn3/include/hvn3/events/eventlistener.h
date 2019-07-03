#pragma once

#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/EventListenerPriority.h"
#include "hvn3/utility/TypeTraits.h"

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>

namespace hvn3 {

	namespace implementation {

		template<typename EventType>
		class EventListenerMethodBase :
			public EventListenerBaseMethodBase<EventType> {

		protected:
			typedef std::function<void(parameter_type)> callback_type;

		public:
			void OnEvent(parameter_type ev) override {

				if (_callback)
					_callback(ev);

			};

		protected:
			void SetCallback(callback_type&& callback) {
				_callback = std::move(callback);
			}

		private:
			callback_type _callback;

		};

		template<typename... FlatEventTypes>
		class EventListenerBase;

		template<typename... FlatEventTypes>
		class EventListenerBase<TypeList<FlatEventTypes...>> :
			public EventListenerMethodBase<FlatEventTypes>... {

		public:
			using event_types = TypeList<FlatEventTypes...>;

			EventListenerBase() = default;

			// Sets the function called when the given event is received.
			template<typename EventType, typename LambdaType>
			void OnEvent(LambdaType callback) {

				static_assert(is_any<EventType, FlatEventTypes...>::value, "The event listener does not handle this event type");

				EventListenerMethodBase<EventType>::SetCallback(std::move(callback));

			}

			// Calls the event handler for the given event type.
			template<typename EventType>
			void OnEvent(EventType& ev) {

				static_assert(is_any<EventType, FlatEventTypes...>::value, "The event listener does not handle this event type");

				EventListenerMethodBase<EventType>::OnEvent(ev);

			}

		};

	}

	class IEventListener {};

	template<typename... EventTypes>
	class EventListener :
		public IEventListener,
		public implementation::EventListenerBase<typename TypeList<EventTypes...>::flatten_type> {
	};

}