#include "events/event.h"

namespace hvn3::events {

	// Public members

	Event::Event() :
		timestamp(0.0),
		underlyingEvent() {
	}
	Event::Event(const underlying_t& event) :
		Event() {

		underlyingEvent = event;

	}
	Event::~Event() {

		// Doing this in the dtor ensures that the user can't forgot to free a user event.

		if (Type() == EventType::UserEvent)
			al_unref_user_event(&GetUnderlyingData()->user);

	}

	EventType Event::Type() const {

		return static_cast<EventType>(underlyingEvent.type);

	}
	double Event::Timestamp() const {

		return timestamp;

	}

	IUserEvent* Event::GetUserEvent() const {

		assert(GetUnderlyingData() != nullptr);

		IUserEvent* userEvent = nullptr;

		if (Type() == EventType::UserEvent && GetUnderlyingData()->user.data1 != 0) {

			userEvent = reinterpret_cast<IUserEvent*>(GetUnderlyingData()->user.data1);

			assert(userEvent != nullptr);

		}

		return userEvent;

	}
	Event::underlying_t* Event::GetUnderlyingData() {

		return &underlyingEvent;

	}
	const Event::underlying_t* Event::GetUnderlyingData() const {

		return &underlyingEvent;

	}

}