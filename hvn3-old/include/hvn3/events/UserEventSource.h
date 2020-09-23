#pragma once

#include "hvn3/events/EventSource.h"
#include "hvn3/events/UserEvent.h"

#include <type_traits>

#include <allegro5/allegro5.h>

namespace hvn3 {

	class UserEventSource :
		public EventSource {

	public:
		UserEventSource();
		~UserEventSource();

		template<typename EventType> typename std::enable_if<!std::is_base_of<IUserEvent, EventType>::value, void>::type
			Push(const EventType& ev);

	private:
		ALLEGRO_EVENT_SOURCE _event_source;

		void _push(IUserEvent* event);

	};

	template<typename EventType> typename std::enable_if<!std::is_base_of<IUserEvent, EventType>::value, void>::type
		UserEventSource::Push(const EventType& ev) {

		using user_event_type = UserEvent<EventType>;

		user_event_type* user_ev = new user_event_type(ev);

		_push(user_ev);

	}

}