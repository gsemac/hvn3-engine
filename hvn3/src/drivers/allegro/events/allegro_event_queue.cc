#include "drivers/allegro/events/allegro_event_queue.h"

#include "core/engine.h"

#include <cassert>

namespace hvn3::events {

	// Public members

	AllegroEventQueue::AllegroEventQueue() {

		core::Engine::Initialize(core::EngineModules::Core);

		eventQueue = al_create_event_queue();

		assert(eventQueue != nullptr);

		RegisterEventSource(userEventSource);

	}
	AllegroEventQueue::AllegroEventQueue(AllegroEventQueue&& other) noexcept {

		core::Engine::Initialize(core::EngineModules::Core);

		eventQueue = other.eventQueue;

		other.eventQueue = nullptr;

	}

	AllegroEventQueue::~AllegroEventQueue() {

		if (eventQueue != nullptr)
			al_destroy_event_queue(eventQueue);

		eventQueue = nullptr;

		core::Engine::Deinitialize(core::EngineModules::Core);

	}

	bool AllegroEventQueue::IsEmpty() const {

		assert(eventQueue != nullptr);

		return	al_is_event_queue_empty(eventQueue);

	}
	bool AllegroEventQueue::GetNextEvent(hvn3::events::Event& ev) {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		return al_get_next_event(eventQueue, ev.GetUnderlyingData());

	}
	bool AllegroEventQueue::PeekNextEvent(hvn3::events::Event& ev) const {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		return al_peek_next_event(eventQueue, ev.GetUnderlyingData());

	}
	bool AllegroEventQueue::WaitForEvent(hvn3::events::Event& ev) {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		al_wait_for_event(eventQueue, ev.GetUnderlyingData());

		return true;

	}
	bool AllegroEventQueue::WaitForEvent(hvn3::events::Event& ev, const TimeSpan& timeout) {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		return al_wait_for_event_timed(eventQueue, ev.GetUnderlyingData(), static_cast<float>(timeout.Seconds()));

	}

	void AllegroEventQueue::PushEvent(const IUserEvent& ev) {

		userEventSource.PushEvent(ev);

	}

	TaggedHandle AllegroEventQueue::GetHandle() const {

		return make_tagged_handle(eventQueue);

	}

	void AllegroEventQueue::RegisterEventSource(const AllegroEventSource& eventSource) {

		assert(eventQueue != nullptr);
		assert(static_cast<bool>(eventSource.GetHandle()));

		al_register_event_source(eventQueue, eventSource.GetHandle());

	}
	void AllegroEventQueue::UnregisterEventSource(const AllegroEventSource& eventSource) {

		assert(eventQueue != nullptr);
		assert(static_cast<bool>(eventSource.GetHandle()));

		al_unregister_event_source(eventQueue, eventSource.GetHandle());

	}


	// Private members

	void AllegroEventQueue::BeforeGetNextEvent(hvn3::events::Event& ev) const {

		if (ev.Type() == hvn3::events::EventType::UserEvent)
			al_unref_user_event(&ev.GetUnderlyingData()->user);

	}

}