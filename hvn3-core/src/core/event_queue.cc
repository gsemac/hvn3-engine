#include "core/engine.h"
#include "events/event_queue.h"

#include <cassert>

namespace hvn3::events {

	// Public members

	EventQueue::EventQueue() {

		core::Engine::Initialize(core::EngineModules::Core);

		eventQueue = al_create_event_queue();

		assert(eventQueue != nullptr);

	}
	EventQueue::EventQueue(EventQueue&& other) noexcept {

		core::Engine::Initialize(core::EngineModules::Core);

		eventQueue = other.eventQueue;

		other.eventQueue = nullptr;

	}

	EventQueue::~EventQueue() {

		if (eventQueue != nullptr)
			al_destroy_event_queue(eventQueue);

		eventQueue = nullptr;

		core::Engine::Deinitialize(core::EngineModules::Core);

	}

	void EventQueue::RegisterEventSource(const IEventSource& eventSource) {

		assert(eventQueue != nullptr);
		assert(eventSource.GetUnderlyingData() != nullptr);

		al_register_event_source(eventQueue, eventSource.GetUnderlyingData());

	}
	void EventQueue::UnregisterEventSource(const IEventSource& eventSource) {

		assert(eventQueue != nullptr);
		assert(eventSource.GetUnderlyingData() != nullptr);

		al_unregister_event_source(eventQueue, eventSource.GetUnderlyingData());

	}

	bool EventQueue::IsEmpty() const {

		assert(eventQueue != nullptr);

		return	al_is_event_queue_empty(eventQueue);

	}
	bool EventQueue::GetNextEvent(Event& ev) {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		return al_get_next_event(eventQueue, ev.GetUnderlyingData());

	}
	bool EventQueue::PeekNextEvent(Event& ev) const {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		return al_peek_next_event(eventQueue, ev.GetUnderlyingData());

	}
	void EventQueue::WaitForEvent(Event& ev) {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		al_wait_for_event(eventQueue, ev.GetUnderlyingData());

	}
	bool EventQueue::WaitForEvent(Event& ev, const core::TimeSpan& timeout) {

		assert(eventQueue != nullptr);

		BeforeGetNextEvent(ev);

		return al_wait_for_event_timed(eventQueue, ev.GetUnderlyingData(), static_cast<float>(timeout.Seconds()));

	}

	EventQueue::underlying_t* EventQueue::GetUnderlyingData() {

		return eventQueue;

	}

	EventQueue::operator bool() const {

		return eventQueue != nullptr;

	}

	// Private members

	void EventQueue::BeforeGetNextEvent(Event& ev) const {

		if (ev.Type() == EventType::UserEvent)
			al_unref_user_event(&ev.GetUnderlyingData()->user);

	}

}