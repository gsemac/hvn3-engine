#pragma once

#include "hvn3/events/IUserEvent.h"

namespace hvn3 {

	template<typename EventType>
	class UserEvent :
		public IUserEvent {

	public:
		typedef EventType event_type;

		UserEvent(const event_type& eventData);
		UserEvent(event_type&& eventData);

		event_id Id() const override;
		void* Data() override;

		static event_id EventId();

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
	typename UserEvent<EventType>::event_id UserEvent<EventType>::Id() const {
		return EventId();
	}
	template<typename EventType>
	void* UserEvent<EventType>::Data() {
		return &_data;
	}

	template<typename EventType>
	typename UserEvent<EventType>::event_id UserEvent<EventType>::EventId() {
		return event_indexer::GetIndex<event_type>();
	}

}