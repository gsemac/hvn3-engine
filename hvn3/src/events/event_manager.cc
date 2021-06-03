#include "events/display_events.h"
#include "events/draw_events.h"
#include "events/event_manager.h"
#include "events/mouse_events.h"
#include "events/timer_events.h"

namespace hvn3::events {

	// Public members

	EventManager::EventManager() :
		pendingFrameDraw(false) {
	}

	IEventQueue& EventManager::GetEventQueue() {

		return *eventQueue.get();

	}
	const IEventQueue& EventManager::GetEventQueue() const {

		return *eventQueue.get();

	}
	MultiEventBus& EventManager::GetEventBus() {

		return eventBus;

	}
	const MultiEventBus& EventManager::GetEventBus() const {

		return eventBus;

	}

	void EventManager::AddEventFilter(const EventFilterHandle& eventFilter) {

		eventFilters.push_back(eventFilter);

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

			if (gotEvent) {

				bool filtered = false;

				for (const auto& eventFilter : eventFilters) {

					filtered = eventFilter->PreFilterEvent(ev);

					if (filtered)
						break;

				}

				if (!filtered)
					DoEvent(ev);

			}

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

		case EventType::MouseAxes:

			OnMouseAxes(ev);

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
	void EventManager::OnMouseAxes(Event& ev) {

		GetEventBus().Dispatch(MouseMoveEvent(GetMousePosition(ev), GetMouseScrollOffset(ev), GetMouseButtons(ev), 0));

	}
	void EventManager::OnTimer(Event& ev) {

		GetEventBus().Dispatch(TickEvent());

		pendingFrameDraw = true;

	}

	math::Point2i EventManager::GetMousePosition(Event& ev) {

		int x = ev.GetUnderlyingData()->mouse.x;
		int y = ev.GetUnderlyingData()->mouse.y;

		return math::Point2i(x, y);

	}
	math::Point2i EventManager::GetMouseScrollOffset(Event& ev) {

		int w = ev.GetUnderlyingData()->mouse.w;
		int z = ev.GetUnderlyingData()->mouse.z;

		return math::Point2i(w, z);

	}
	io::MouseButton EventManager::GetMouseButtons(Event& ev) {

		int button = ev.GetUnderlyingData()->mouse.button;
		io::MouseButton buttons = io::MouseButton::None;

		if (button & 1)
			buttons |= io::MouseButton::Left;

		if (button & 2)
			buttons |= io::MouseButton::Right;

		if (button & 4)
			buttons |= io::MouseButton::Middle;

		return buttons;

	}

}