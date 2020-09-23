#include "hvn3/events/Event.h"
#include "hvn3/events/UserEventSource.h"
#include <iostream>
namespace hvn3 {

	constexpr ALLEGRO_EVENT_TYPE ALLEGRO_USER_EVENT_TYPE = 1024;

	// Helper functions

	void freeEvent(ALLEGRO_USER_EVENT* ev) {

		assert(ev != nullptr);

		IUserEvent* ptr = reinterpret_cast<IUserEvent*>(ev->data1);

		assert(ptr != nullptr);

		delete ptr;

	}

	// Public methods

	UserEventSource::UserEventSource() :
		EventSource(&_event_source) {

		al_init_user_event_source(&_event_source);

	}
	UserEventSource::~UserEventSource() {

		al_destroy_user_event_source(&_event_source);

	}

	// Private methods

	void UserEventSource::_push(IUserEvent* event) {

		assert(event != nullptr);

		ALLEGRO_EVENT ev;
		ev.type = (ALLEGRO_EVENT_TYPE)EventType::UserEvent;
		ev.user.data1 = reinterpret_cast<decltype(ev.user.data1)>(event);

		al_emit_user_event(&_event_source, &ev, freeEvent);

	}

}