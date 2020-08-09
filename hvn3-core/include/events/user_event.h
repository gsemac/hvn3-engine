#pragma once

#include "events/multi_event_listener_container.h"
#include "events/iuser_event.h"

namespace hvn3::events {

	template<typename EventType>
	class UserEvent :
		public IUserEvent {

	public:
		typedef EventType event_type;

		UserEvent(const event_type& eventData);
		UserEvent(event_type&& eventData);

		event_id_type Id() const override;
		void* Data() override;

		void Dispatch(const MultiEventListenerContainer& eventListenerContainer) override;

		static event_id_type EventId();

	private:
		event_type _data;

	};

	// Public methods

	template<typename EventType>
	UserEvent<EventType>::UserEvent(const event_type& eventData) :
		_data(eventData) {
	}
	template<typename EventType>
	UserEvent<EventType>::UserEvent(event_type&& eventData) :
		_data(eventData) {
	}

	template<typename EventType>
	typename UserEvent<EventType>::event_id_type UserEvent<EventType>::Id() const {

		return EventId();

	}
	template<typename EventType>
	void* UserEvent<EventType>::Data() {

		return &_data;

	}
	template<typename EventType>
	void UserEvent<EventType>::Dispatch(const MultiEventListenerContainer& eventListenerContainer) {

		registry->Dispatch<EventType>(_data);

	}

	template<typename EventType>
	typename UserEvent<EventType>::event_id_type UserEvent<EventType>::EventId() {

		return event_index<event_type>();

	}

}