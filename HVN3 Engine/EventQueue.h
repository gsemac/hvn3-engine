#pragma once

class Event;
class EventSource;
struct ALLEGRO_EVENT_QUEUE;

class EventQueue {

public:
	EventQueue();
	~EventQueue();

	// Pauses accepting new events into the queue.
	void Pause();
	// Resumes accepting new events into the queue.
	void Resume();
	// Returns true if accepting events is paused.
	bool IsPaused() const;	
	// Returns true if the event queue is empty.
	bool IsEmpty() const;
	
	// Causes the event queue to listen to events from the specified source.
	void AddEventSource(const EventSource& source);
	// Causes the event queue to stop listening to events from the specified source.
	void RemoveEventSource(const EventSource& source);
	// Returns true if the event queue is listening to events from the specified source.
	bool ListeningToEventSource(const EventSource& source);

	// Gets the next event from the queue, and pops it from the queue. Returns false if the queue is empty.
	bool GetNextEvent(Event& ev);
	// Gets the next event from the queue without popping it from the queue. Returns false if the queue is empty.
	bool PeekNextEvent(Event& ev) const;
	// Drops the next event from the queue. Returns false if the queue is empty.
	bool DropNextEvent();
	// Drops all events from the event queue.
	void Flush();
	// Waits until the queue is non-empty, and gets the next event from the queue.
	void WaitForEvent(Event& ev);
	// Waits until the queue is non-empty or timeout occurs, and gets the next event from the queue. If timeout occurs, returns false.
	bool WaitForEvent(Event& ev, float seconds);

private:
	ALLEGRO_EVENT_QUEUE* __event_queue;

};