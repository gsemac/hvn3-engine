#pragma once

#include "events/multi_event_bus.h"
#include "events/iuser_event.h"

#include <utility>
#include <type_traits>

namespace hvn3::events {

	template<typename EventType>
	class UserEvent :
		public IUserEvent {

	public:
		typedef EventType event_type;

		UserEvent(const event_type& eventData);
		UserEvent(event_type&& eventData);

		event_id_type Id() const;
		void* Data() override;
		IUserEvent* Clone() const override;

		void DispatchTo(MultiEventBus& eventBus) override;

		events::EventType Type() const override;
		double Timestamp() const override;

		static event_id_type EventId();

	private:
		event_type data;

	};

	// Public methods

	template<typename EventType>
	UserEvent<EventType>::UserEvent(const event_type& eventData) :
		data(eventData) {
	}
	template<typename EventType>
	UserEvent<EventType>::UserEvent(event_type&& eventData) :
		data(eventData) {
	}

	template<typename EventType>
	typename UserEvent<EventType>::event_id_type UserEvent<EventType>::Id() const {

		return EventId();

	}
	template<typename EventType>
	void* UserEvent<EventType>::Data() {

		return &data;

	}
	template<typename EventType>
	IUserEvent* UserEvent<EventType>::Clone() const {

		return new UserEvent<EventType>(data);

	}
	template<typename EventType>
	void UserEvent<EventType>::DispatchTo(MultiEventBus& eventBus) {

		eventBus.Dispatch<EventType>(data);

	}

	template<typename EventType>
	events::EventType UserEvent<EventType>::Type() const {

		return events::EventType::UserEvent;

	}
	template<typename EventType>
	double UserEvent<EventType>::Timestamp() const {

		return 0.0;

	}

	template<typename EventType>
	typename UserEvent<EventType>::event_id_type UserEvent<EventType>::EventId() {

		return event_index<event_type>();

	}

	template<typename EventType, typename Arg0, typename ...Args>
	UserEvent<EventType> make_event(Arg0&& arg0, Args&&... args) {

		return UserEvent<EventType>(EventType(std::forward<Arg0&&>(arg0), std::forward<Args&&>(args)...));

	}
	template<typename EventType>
	UserEvent<std::remove_reference_t<EventType>> make_event(EventType&& eventData) {

		return UserEvent<std::remove_reference_t<EventType>>(std::forward<EventType&&>(eventData));

	}

}