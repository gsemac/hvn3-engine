#include "EventQueue.h"
#include "EventSource.h"
#include "Event.h"
#include <allegro5/allegro.h>

EventQueue::EventQueue() {

	__event_queue = al_create_event_queue();
	if (__event_queue == NULL)
		__event_queue = nullptr;

}
EventQueue::~EventQueue() {

	if (__event_queue)
		al_destroy_event_queue(__event_queue);
	__event_queue = nullptr;

}

void EventQueue::Pause() {

	al_pause_event_queue(__event_queue, true);

}
void EventQueue::Resume() {

	al_pause_event_queue(__event_queue, false);

}
bool EventQueue::IsPaused() const {

	return al_is_event_queue_paused(__event_queue);

}
bool EventQueue::IsEmpty() const {

	return al_is_event_queue_empty(__event_queue);

}

void EventQueue::AddEventSource(const EventSource& source) {

	al_register_event_source(__event_queue, source.__ev_source);

}
void EventQueue::RemoveEventSource(const EventSource& source) {

	al_unregister_event_source(__event_queue, source.__ev_source);

}
bool EventQueue::ListeningToEventSource(const EventSource& source) {

	return al_is_event_source_registered(__event_queue, source.__ev_source);

}

bool EventQueue::GetNextEvent(Event& ev) {

	// Get the next event.
	return al_get_next_event(__event_queue, &ev.__ev);

}
bool EventQueue::PeekNextEvent(Event& ev) const {

	// Peek the next event.
	return al_peek_next_event(__event_queue, &ev.__ev);

}
bool EventQueue::DropNextEvent() {

	return al_drop_next_event(__event_queue);

}
void EventQueue::Flush() {

	al_flush_event_queue(__event_queue);

}
void EventQueue::WaitForEvent(Event& ev) {

	al_wait_for_event(__event_queue, &ev.__ev);

}
bool EventQueue::WaitForEvent(Event& ev, float seconds) {

	return al_wait_for_event_timed(__event_queue, &ev.__ev, seconds);

}