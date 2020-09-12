#include "core/engine.h"
#include "events/user_event_source.h"

#include <allegro5/allegro.h>

#include <cassert>

namespace hvn3::events {

	// Public members

	UserEventSource::UserEventSource() {

		core::Engine::Initialize(core::EngineModules::Core);

		eventSource = std::make_shared<EventSourceWrapper>();

	}

	void UserEventSource::PushEvent(const IUserEvent& event) const {

		// We'll push a copy of the event onto the queue so we can be certain of its lifetime.

		PushEvent(event.Clone());

	}

	ALLEGRO_EVENT_SOURCE* UserEventSource::GetUnderlyingData() const {

		return &eventSource->eventSource;

	}

	UserEventSource::operator bool() const {

		return static_cast<bool>(eventSource);

	}

	// Private members

	UserEventSource::EventSourceWrapper::EventSourceWrapper() {

		al_init_user_event_source(&eventSource);

	}
	UserEventSource::EventSourceWrapper::~EventSourceWrapper() {

		al_destroy_user_event_source(&eventSource);

		core::Engine::Deinitialize(core::EngineModules::Core);

	}

	void UserEventSource::PushEvent(IUserEvent* event) const {

		assert(event != nullptr);

		ALLEGRO_EVENT allegroEvent;

		using DataPtrType = decltype(allegroEvent.user.data1);

		allegroEvent.type = (ALLEGRO_EVENT_TYPE)EventType::UserEvent;
		allegroEvent.user.data1 = reinterpret_cast<DataPtrType>(event);

		al_emit_user_event(&eventSource->eventSource, &allegroEvent, FreeEvent);

	}

	void UserEventSource::FreeEvent(ALLEGRO_USER_EVENT* event) {

		assert(event != nullptr);

		if (event != nullptr) {

			assert(event->data1 != 0);

			IUserEvent* userEventData = reinterpret_cast<IUserEvent*>(event->data1);

			assert(userEventData != nullptr);

			delete userEventData;

		}

	}

}