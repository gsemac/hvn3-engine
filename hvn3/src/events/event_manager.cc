#include "events/display_events.h"
#include "events/event_manager.h"
#include "events/graphics_events.h"
#include "events/timer_events.h"

namespace hvn3::events {

	// Public members

	EventManager::EventManager() :
		pendingFrameDraw(false) {
	}

	IEventQueue& EventManager::GetEventQueue() {

		return eventQueue;

	}
	const IEventQueue& EventManager::GetEventQueue() const {

		return eventQueue;

	}
	MultiEventBus& EventManager::GetEventBus() {

		return eventBus;

	}
	const MultiEventBus& EventManager::GetEventBus() const {

		return eventBus;

	}

	bool EventManager::DoEvents(bool waitForEvent) {

		bool gotEvent = false;

		Event ev;

		do {

			// Get the next event from the event queue.
			// If we're waiting for an event, we'll only wait for the first one.

			gotEvent = waitForEvent ?
				GetEventQueue().WaitForEvent(ev) :
				GetEventQueue().GetNextEvent(ev);

			waitForEvent = false;

			if (gotEvent)
				DoEvent(ev);

		} while (gotEvent);

		// If the event queue is empty, allow the next frame to be drawn.

		if (pendingFrameDraw && GetEventQueue().IsEmpty())
			OnFrameDraw();

		// Indicate to the caller whether or not any events were received.

		return gotEvent;

	}

	// Private members

	void EventManager::DoEvent(Event& ev) {

		switch (ev.Type()) {

		case EventType::DisplayClose:

			OnDisplayClose(ev);

			break;

		case EventType::Timer:

			OnTimer(ev);

			break;

		}

	}

	void EventManager::OnDisplayClose(Event& ev) {

		GetEventBus().Dispatch(DisplayCloseEvent());

	}
	void EventManager::OnFrameDraw() {

		GetEventBus().Dispatch(DrawFrameEvent());

		pendingFrameDraw = false;

	}
	void EventManager::OnTimer(Event& ev) {

		GetEventBus().Dispatch(TickEvent());

		pendingFrameDraw = true;

	}

}