#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/events/Event.h"
#include "hvn3/events/EventQueue.h"
#include "hvn3/events/EventSource.h"
#include "hvn3/events/IEventManager.h"
#include "hvn3/events/Timer.h"
#include "hvn3/io/KeyboardState.h"
#include "hvn3/io/MouseState.h"
#include "hvn3/utility/FpsCounter.h"
#include "hvn3/utility/Stopwatch.h"

#include <type_traits>
#include <vector>

namespace hvn3 {

	class IDisplayListener;
	class IKeyboardListener;
	class IMouseListener;
	class IUpdatable;

	class EventManager :
		public IEventManager {

	public:
		EventManager();
		~EventManager();

		void RegisterEventSource(EventSource& eventSource);

		template<typename ListenerType>
		void RegisterEventListener(ListenerType* listener) {

			// #todo Use enable_if or a similar mechanism to avoid having to use an unsafe cast.

			if (std::is_base_of<IUpdatable, ListenerType>::value)
				_update_listeners.push_back((IUpdatable*)listener);

		}
		template<typename ListenerType>
		void DeregisterEventListener(ListenerType* listener) {}

		bool IsEmpty() const;
		bool IsSuspendable() const override;

		int MaxFrameSkip() const {}
		void SetMaxFrameSkip(int value) {}

		// Handles all events in the event queue, dispatching them to the appropriate listeners.
		// Does not return until all events have been handled.
		void DoEvents(bool blocking);

	protected:
		void DoEvent(Event& event);

		void OnTimerTick(Event& ev);
		void OnDisplayClose(Event& ev);
		void OnKeyDown(Event& ev);
		void OnKeyUp(Event& ev);
		void OnKeyChar(Event& ev);
		void OnMouseButtonDown(Event& ev);
		void OnMouseButtonUp(Event& ev);
		void OnMouseAxes(Event& ev);
		void OnMouseEnterDisplay(Event& ev);
		void OnMouseLeaveDisplay(Event& ev);
		void OnDisplayResize(Event& ev);
		void OnDisplaySwitchOut(Event& ev);
		void OnDisplaySwitchIn(Event& ev);

		void ReceiveContext(const ApplicationContext & context) override;
		ApplicationContext Context() override;

	private:
		void _doGlobalMouseTracking();

		bool _is_first_update;
		EventQueue _event_queue;
		int _frames_skipped;
		int _max_frame_skip; // Used to decide when to skip updates to avoid overwhelming the event queue during consistently long updates
		Stopwatch _update_delta_timer; // Used to measure the time between updates (passed in update args)

		bool _global_mouse_tracking_enabled;
		bool _global_mouse_tracking_required; // The mouse is tracked manually when this is enabled (only set when the mouse is not in a display)

		KeyboardState _keyboard_state;
		MouseState _mouse_state;

		//Timer _frame_rate_timer; // generates timer tick events
		//bool _frame_rate_locked; // whether or not the frame rate is fixed
		//Stopwatch _update_delta_timer; // used to measure the time between updates (passed in update args)
		//FpsCounter _fps_counter; // used to measure frames per second

		std::vector<IDisplayListener*> _display_listeners;
		std::vector<IKeyboardListener*> _keyboard_listeners;
		std::vector<IMouseListener*> _mouse_listeners;
		std::vector<IUpdatable*> _update_listeners;

	};

}