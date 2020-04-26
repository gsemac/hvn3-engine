#include "hvn3/core/Engine.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/events/UpdateEventArgs.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/io/IKeyboardListener.h"
#include "hvn3/io/IMouseListener.h"
#include "hvn3/io/IOUtils.h"
#include "hvn3/io/KeyboardEventArgs.h"
#include "hvn3/io/window_events.h"

namespace hvn3 {

	// Public methods

	EventManager::EventManager() {

		// Initialize the required engine components (if necessary).

		core::Engine::Initialize(core::EngineModules::Core | core::EngineModules::IO);

		// Initialize other members.

		_is_first_update = true;
		_frames_skipped = 0;
		_redraw_required = false;
		_max_frame_skip = 10;
		_global_mouse_tracking_enabled = true;
		_global_mouse_tracking_required = false;

		RegisterEventSource(_user_event_source);

	}
	EventManager::~EventManager() {

		// Release the engine components.

		core::Engine::Initialize(core::EngineModules::Core | core::EngineModules::IO);

	}

	void EventManager::RegisterEventSource(EventSource& eventSource) {
		_event_queue.AddEventSource(eventSource);
	}

	bool EventManager::IsEmpty() const {
		return _event_queue.IsEmpty();
	}

	bool EventManager::RedrawRequired() const {
		return _redraw_required;
	}

	void EventManager::DoEvents(bool blocking) {

		Event event;

		_frames_skipped = 0;
		_redraw_required = false;

		while (true) {

			// Get the next event from the queue, blocking until one appears if desired by the caller.

			if (blocking)
				_event_queue.WaitForEvent(event);
			else if (!_event_queue.GetNextEvent(event))
				break;

			// Handle the next event.
			DoEvent(event);

			// When the event queue is empty, all events have been handled, so exit the loop.

			if (_event_queue.IsEmpty())
				break;

		}

	}

	EventListenerRegistry& EventManager::GetListenerRegistry() {
		return *this;
	}

	// Protected methods

	void EventManager::DoEvent(Event& event) {

		switch (event.Type()) {

		case EventType::Timer:
			OnTimerTick(event);
			break;

		case EventType::DisplayClose:
			OnDisplayClose(event);
			break;

		case EventType::KeyDown:
			OnKeyDown(event);
			break;

		case EventType::KeyUp:
			OnKeyUp(event);
			break;

		case EventType::KeyChar:
			OnKeyChar(event);
			break;

		case EventType::MouseButtonDown:
			OnMouseButtonDown(event);
			break;

		case EventType::MouseButtonUp:
			OnMouseButtonUp(event);
			break;

		case EventType::MouseAxes:
			OnMouseAxes(event);
			break;

		case EventType::MouseEnterDisplay:
			OnMouseEnterDisplay(event);
			break;

		case EventType::MouseLeaveDisplay:
			OnMouseLeaveDisplay(event);
			break;

		case EventType::DisplayResize:
			OnDisplayResize(event);
			break;

		case EventType::DisplaySwitchOut:
			OnDisplaySwitchOut(event);
			break;

		case EventType::DisplaySwitchIn:
			OnDisplaySwitchIn(event);
			break;

		case EventType::UserEvent:
			OnUserEvent(event);
			break;

		}

	}

	void EventManager::OnTimerTick(Event& ev) {

		// If this is the first update, reset the update delta timer.

		if (_is_first_update) {

			_update_delta_timer.Clear();
			_update_delta_timer.Start();

			_is_first_update = false;

		}

		// If the global mouse position needs to be tracked (i.e., it's outside of a display), update its position.
		_doGlobalMouseTracking();

		// Notify all update listeners.

		double seconds_elapsed = _update_delta_timer.SecondsElapsed();

		// Pre-update
		Dispatch<UpdateBeginEventArgs>(seconds_elapsed);

		// Update
		Dispatch<UpdateEventArgs>(seconds_elapsed);

		// For any keys actively held, notify all keyboard listeners.

		if (_keyboard_state.KeyDown(Key::Any)) {

			for (auto i = _keyboard_state.begin(); i != _keyboard_state.end(); ++i)
				if (i->Held())
					Dispatch<KeyDownEventArgs>(i->Key(), _keyboard_state.Modifiers());

		}

		// For any mouse buttons actively held, notify all mouse listeners.

		for (auto i = _mouse_state.begin(); i != _mouse_state.end(); ++i)
			if (i->Held())
				Dispatch<IMouseListener::MouseDownEventArgs>(i->Button(), _mouse_state.DisplayPosition(), _mouse_state.Position());

		// Post-update
		Dispatch<UpdateEndEventArgs>(seconds_elapsed);

		// Unset all pressed/released keys so the values will be false until next triggered.
		_keyboard_state.ClearKeyStates(true, true, false);

		// Unset all pressed/released mouse buttons so the values will be false until next triggered.
		_mouse_state.ClearButtonStates(true, true, false);

		// Reset the delta timer.
		_update_delta_timer.Clear();

		// Since the game state has been updated, it needs to be redrawn.
		_redraw_required = true;

	}
	void EventManager::OnDisplayClose(Event& ev) {

		// Notify all display listeners.
		Dispatch<io::WindowClosedEventArgs>(io::Window(nullptr, false)); // #todo should not be null, but the actual display

	}
	void EventManager::OnKeyDown(Event& ev) {

		// Update the keyboard state.

		int key_code = ev.AlPtr()->keyboard.keycode;
		_keyboard_state.SetKeyState(key_code, true);

		// Notify all keyboard listeners.

		if (_keyboard_state.KeyPressed(key_code))
			Dispatch<KeyPressedEventArgs>(static_cast<Key>(key_code), _keyboard_state.Modifiers());

	}
	void EventManager::OnKeyUp(Event& ev) {

		// Update the keyboard state.

		int key_code = ev.AlPtr()->keyboard.keycode;
		_keyboard_state.SetKeyState(key_code, false);

		// Notify all keyboard listeners.
		Dispatch<KeyUpEventArgs>(static_cast<Key>(key_code), _keyboard_state.Modifiers());

	}
	void EventManager::OnKeyChar(Event& ev) {

		// Note that non-printable key codes like arrows, delete, backspace, etc. are also returned here.
		// https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_char

		// Update the keyboard state.

		int32_t character = ev.AlPtr()->keyboard.unichar;

		if (io::IOUtils::IsPrintableChar(character))
			_keyboard_state.SetLastChar(character);

		// Notify all keyboard listeners.
		Dispatch<KeyCharEventArgs>(static_cast<Key>(ev.AlPtr()->keyboard.keycode), _keyboard_state.Modifiers(), character);

	}
	void EventManager::OnMouseButtonDown(Event& ev) {

		// Update the mouse state.

		MouseButton button;

		switch (ev.AlPtr()->mouse.button) {

		case 1:
			button = MouseButton::Left;
			break;

		case 2:
			button = MouseButton::Right;
			break;

		case 3:
			button = MouseButton::Middle;
			break;

		}

		_mouse_state.SetButtonState(button, true);

		// Notify all mouse listeners.
		Dispatch<IMouseListener::MousePressedEventArgs>(button, _mouse_state.DisplayPosition(), _mouse_state.Position(), _mouse_state.ButtonDoubleClicked(button) ? 2 : 1);

	}
	void EventManager::OnMouseButtonUp(Event& ev) {

		// Update the mouse state.

		MouseButton button;

		switch (ev.AlPtr()->mouse.button) {

		case 1:
			button = MouseButton::Left;
			break;

		case 2:
			button = MouseButton::Right;
			break;

		case 3:
			button = MouseButton::Middle;
			break;

		}

		_mouse_state.SetButtonState(button, false);

		// Notify all mouse listeners.
		Dispatch<IMouseListener::MouseReleasedEventArgs>(button, _mouse_state.DisplayPosition(), _mouse_state.Position());

	}
	void EventManager::OnMouseAxes(Event& ev) {

		ALLEGRO_MOUSE_EVENT& mouse = ev.AlPtr()->mouse;

		// Update the mouse state.
		// Only the mouse' display position will be set here; MouseState will automatically update the world position.

		_mouse_state.SetDisplayPosition(mouse.x, mouse.y);

		if (mouse.dw != 0 || mouse.dz != 0)
			_mouse_state.SetScrollState(mouse.dz < 0, mouse.dz > 0);

		// Notify all mouse listeners.

		Dispatch<IMouseListener::MouseMoveEventArgs>(_mouse_state.DisplayPosition(), _mouse_state.Position());

		if (mouse.dw != 0 || mouse.dz != 0)
			Dispatch<IMouseListener::MouseScrollEventArgs>(_mouse_state.DisplayPosition(), _mouse_state.Position(), PointI(mouse.w, mouse.z), mouse.dw, mouse.dz);

	}
	void EventManager::OnMouseEnterDisplay(Event& ev) {

		OnMouseAxes(ev);

		_global_mouse_tracking_required = false;

	}
	void EventManager::OnMouseLeaveDisplay(Event& ev) {
		_global_mouse_tracking_required = true;
	}
	void EventManager::OnDisplayResize(Event& ev) {

		// Create a non-owning display for us to work with.

		io::Window display(ev.AlPtr()->display.source, false);

		// Store the old size so that we can include it in the event args.
		//SizeI old_size = display.Size();

		// Acknowledge the resize.

		al_acknowledge_resize(ev.AlPtr()->display.source);

		// Notify all listeners.

		Dispatch<io::WindowSizeChangedEventArgs>(display);

	}
	void EventManager::OnDisplaySwitchOut(Event& ev) {

		// Create a non-owning display for us to work with.

		io::Window display(ev.AlPtr()->display.source, false);

		// Reset mouse/keyboard IO so it's back to the default state when the display is switched back in.

		_keyboard_state.ClearKeyStates();
		_mouse_state.ClearButtonStates();

		// Notify all listeners.

		KeyboardLostEventArgs keyboard_args;

		Dispatch<KeyboardLostEventArgs>();
		Dispatch<io::WindowLostEventArgs>(display);

	}
	void EventManager::OnDisplaySwitchIn(Event& ev) {

		// Create a non-owning display for us to work with.

		io::Window display(ev.AlPtr()->display.source, false);

		// Notify all listeners.

		Dispatch<KeyboardFoundEventArgs>();
		Dispatch<io::WindowFoundEventArgs>(display);

	}
	void EventManager::OnUserEvent(Event& ev) {

		IUserEvent* user_ev = ev.GetUserEvent();

		assert(user_ev != nullptr);

		Dispatch(*user_ev);

	}

	// Private methods

	void EventManager::_doGlobalMouseTracking() {

		if (_global_mouse_tracking_enabled && _global_mouse_tracking_required) {

			// Update the mouse position according to its global position outside of the display.

			int x, y;
			al_get_mouse_cursor_position(&x, &y);

			_mouse_state.SetDisplayPosition(x, y);

			// Notify all mouse listeners.

			IMouseListener::MouseMoveEventArgs args(_mouse_state.DisplayPosition(), _mouse_state.Position());

			Dispatch(args);

		}

	}

}