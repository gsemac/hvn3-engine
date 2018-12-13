#include "hvn3/core/Engine.h"
#include "hvn3/events/EventQueue.h"
#include "hvn3/events/EventSource.h"
#include "hvn3/events/Event.h"

#include <allegro5/allegro.h>
#include <cassert>

namespace hvn3 {

	EventQueue::EventQueue() {

		System::Engine::RequireComponent(System::EngineComponent::Core);

		_event_queue = al_create_event_queue();

		assert(_event_queue != NULL);

	}
	EventQueue::EventQueue(EventQueue&& other) {

		_event_queue = other._event_queue;

		other._event_queue = nullptr;

	}
	EventQueue::~EventQueue() {

		if (_event_queue)
			al_destroy_event_queue(_event_queue);

		_event_queue = nullptr;

		System::Engine::ReleaseComponent(System::EngineComponent::Core);

	}
	void EventQueue::Pause() {

		al_pause_event_queue(_event_queue, true);

	}
	void EventQueue::Resume() {

		al_pause_event_queue(_event_queue, false);

	}
	bool EventQueue::IsPaused() const {

		return al_is_event_queue_paused(_event_queue);

	}
	bool EventQueue::IsEmpty() const {

		return al_is_event_queue_empty(_event_queue);

	}
	void EventQueue::AddEventSource(const EventSource& source) {

		al_register_event_source(_event_queue, source._event_source);

	}
	void EventQueue::RemoveEventSource(const EventSource& source) {

		al_unregister_event_source(_event_queue, source._event_source);

	}
	bool EventQueue::ListeningToEventSource(const EventSource& source) {

		return al_is_event_source_registered(_event_queue, source._event_source);

	}
	bool EventQueue::GetNextEvent(Event& ev) {

		// Get the next event.
		return al_get_next_event(_event_queue, &ev._ev);

	}
	bool EventQueue::PeekNextEvent(Event& ev) const {

		// Peek the next event.
		return al_peek_next_event(_event_queue, &ev._ev);

	}
	bool EventQueue::DropNextEvent() {

		return al_drop_next_event(_event_queue);

	}
	void EventQueue::Flush() {

		al_flush_event_queue(_event_queue);

	}
	void EventQueue::WaitForEvent(Event& ev) {

		al_wait_for_event(_event_queue, &ev._ev);

	}
	bool EventQueue::WaitForEvent(Event& ev, float seconds) {

		return al_wait_for_event_timed(_event_queue, &ev._ev, seconds);

	}

}