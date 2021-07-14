#include "events/allegro_event_source.h"

#include "core/engine.h"
#include "events/iuser_event.h"

#include <algorithm>
#include <cassert>

namespace hvn3::events {

	// Public members

	AllegroEventSource::AllegroEventSource() :
		AllegroEventSource(new underlying_t{}, true) {

		core::Engine::Initialize(core::EngineModules::Core);

		al_init_user_event_source(eventSource.get());

		canPushEvents = true;

	}
	AllegroEventSource::AllegroEventSource(underlying_t* eventSource, bool free) {

		if (free)
			this->eventSource = std::shared_ptr<underlying_t>(eventSource, FreeEventSource);
		else
			this->eventSource = std::shared_ptr<underlying_t>(std::shared_ptr<underlying_t>{}, eventSource);

		canPushEvents = false;

	}
	AllegroEventSource::AllegroEventSource(const std::shared_ptr<underlying_t>& eventSource) :
		eventSource(eventSource) {

		canPushEvents = false;

	}

	void AllegroEventSource::PushEvent(const IUserEvent& ev) const {

		assert(canPushEvents);

		// We'll push a copy of the event onto the queue so we can be certain of its lifetime.

		PushEvent(ev.Clone());

	}

	AllegroEventSource::operator bool() const {

		return static_cast<bool>(GetHandle());

	}

	TaggedHandle AllegroEventSource::GetHandle() const {

		return make_tagged_handle(eventSource.get());

	}

	// Private members

	void AllegroEventSource::FreeEventSource(underlying_t* userEventSource) {

		al_destroy_user_event_source(userEventSource);

		// Assume that since the user has passed ownership to us, it's up to us to do the deinitialization.

		core::Engine::Deinitialize(core::EngineModules::Core);

	}
	void AllegroEventSource::PushEvent(IUserEvent* event) const {

		assert(event != nullptr);

		ALLEGRO_EVENT allegroEvent{};

		using DataPtrType = decltype(allegroEvent.user.data1);

		allegroEvent.type = (ALLEGRO_EVENT_TYPE)hvn3::events::EventType::UserEvent;
		allegroEvent.user.data1 = reinterpret_cast<DataPtrType>(event);

		al_emit_user_event(eventSource.get(), &allegroEvent, FreeEvent);

	}
	void AllegroEventSource::FreeEvent(ALLEGRO_USER_EVENT* event) {

		assert(event != nullptr);

		if (event != nullptr) {

			assert(event->data1 != 0);

			hvn3::events::IUserEvent* userEventData = reinterpret_cast<hvn3::events::IUserEvent*>(event->data1);

			assert(userEventData != nullptr);

			delete userEventData;

		}

	}

}