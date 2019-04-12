#include "hvn3/core/Engine.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/io/IDisplayListener.h"
#include "hvn3/io/IKeyboardListener.h"
#include "hvn3/io/IMouseListener.h"
#include "hvn3/io/IOUtils.h"
#include "hvn3/io/KeyboardEventArgs.h"
#include "hvn3/io/MouseEventArgs.h"

namespace hvn3 {

	// Public methods

	EventManager::EventManager() {

		// Initialize the required engine components (if necessary).
		System::Engine::RequireComponent(System::EngineComponent::Core | System::EngineComponent::IO);

		// Initialize other members.

		_is_first_update = true;
		_frames_skipped = 0;
		_redraw_required = false;
		_max_frame_skip = 10;
		_global_mouse_tracking_enabled = true;
		_global_mouse_tracking_required = false;

	}
	EventManager::~EventManager() {

		// Release the engine components.
		System::Engine::ReleaseComponent(System::EngineComponent::Core | System::EngineComponent::IO);

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

	void EventManager::DispatchEvent(DrawEventArgs& event) {

		for (auto i = _draw_listeners.begin(); i != _draw_listeners.end(); ++i)
			(*i)->OnDraw(event);

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

		}

	}

	void EventManager::OnTimerTick(Event& ev) {

		// If this is the first update, reset the update delta timer.

		if (_is_first_update) {

			_update_delta_timer.Reset();
			_update_delta_timer.Start();

			_is_first_update = false;

		}

		// If the global mouse position needs to be tracked (i.e., it's outside of a display), update its position.
		_doGlobalMouseTracking();

		// Notify all update listeners.

		UpdateEventArgs args(_update_delta_timer.SecondsElapsed());

		// Pre-update

		for (auto i = _update_listeners.begin(); i != _update_listeners.end(); ++i)
			(*i)->OnBeginUpdate(args);

		// Update

		for (auto i = _update_listeners.begin(); i != _update_listeners.end(); ++i)
			(*i)->OnUpdate(args);

		// For any keys actively held, notify all keyboard listeners.

		if (_keyboard_state.KeyDown(Key::Any)) {

			for (auto i = _keyboard_state.begin(); i != _keyboard_state.end(); ++i)
				if (i->Held()) {

					KeyDownEventArgs args(i->Key(), _keyboard_state.Modifiers());

					for (auto i = _keyboard_listeners.begin(); i != _keyboard_listeners.end(); ++i)
						(*i)->OnKeyDown(args);

				}

		}

		// For any mouse buttons actively held, notify all mouse listeners.

		for (auto i = _mouse_state.begin(); i != _mouse_state.end(); ++i)
			if (i->Held()) {

				MouseDownEventArgs args(i->Button(), _mouse_state.DisplayPosition(), _mouse_state.Position());

				for (auto i = _mouse_listeners.begin(); i != _mouse_listeners.end(); ++i)
					(*i)->OnMouseDown(args);

			}

		// Post-update

		for (auto i = _update_listeners.begin(); i != _update_listeners.end(); ++i)
			(*i)->OnEndUpdate(args);

		// Unset all pressed/released keys so the values will be false until next triggered.
		_keyboard_state.ClearKeyStates(true, true, false);

		// Unset all pressed/released mouse buttons so the values will be false until next triggered.
		_mouse_state.ClearButtonStates(true, true, false);

		// Reset the delta timer.
		_update_delta_timer.Reset();

		// Since the game state has been updated, it needs to be redrawn.
		_redraw_required = true;

	}
	void EventManager::OnDisplayClose(Event& ev) {

		// Notify all display listeners.

		IDisplayListener::DisplayClosedEventArgs args(nullptr); // #todo should not be null, but the actual display

		for (auto i = _display_listeners.begin(); i != _display_listeners.end(); ++i)
			(*i)->OnDisplayClosed(args);

	}
	void EventManager::OnKeyDown(Event& ev) {

		// Update the keyboard state.

		int key_code = ev.AlPtr()->keyboard.keycode;
		_keyboard_state.SetKeyState(key_code, true);

		// Notify all keyboard listeners.

		if (_keyboard_state.KeyPressed(key_code)) {

			KeyPressedEventArgs args(static_cast<Key>(key_code), _keyboard_state.Modifiers());

			for (auto i = _keyboard_listeners.begin(); i != _keyboard_listeners.end(); ++i)
				(*i)->OnKeyPressed(args);

		}

	}
	void EventManager::OnKeyUp(Event& ev) {

		// Update the keyboard state.

		int key_code = ev.AlPtr()->keyboard.keycode;
		_keyboard_state.SetKeyState(key_code, false);

		// Notify all keyboard listeners.

		KeyUpEventArgs args(static_cast<Key>(key_code), _keyboard_state.Modifiers());

		for (auto i = _keyboard_listeners.begin(); i != _keyboard_listeners.end(); ++i)
			(*i)->OnKeyUp(args);

	}
	void EventManager::OnKeyChar(Event& ev) {

		// Note that non-printable key codes like arrows, delete, backspace, etc. are also returned here.
		// https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_char

		// Update the keyboard state.

		int32_t character = ev.AlPtr()->keyboard.unichar;

		if (io::IOUtils::IsPrintableChar(character))
			_keyboard_state.SetLastChar(character);

		// Notify all keyboard listeners.

		KeyCharEventArgs args(static_cast<Key>(ev.AlPtr()->keyboard.keycode), _keyboard_state.Modifiers(), character);

		for (auto i = _keyboard_listeners.begin(); i != _keyboard_listeners.end(); ++i)
			(*i)->OnKeyChar(args);

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

		if (_mouse_state.ButtonPressed(button)) {

			MousePressedEventArgs args(button, _mouse_state.DisplayPosition(), _mouse_state.Position(), _mouse_state.ButtonDoubleClicked(button) ? 2 : 1);

			for (auto i = _mouse_listeners.begin(); i != _mouse_listeners.end(); ++i)
				(*i)->OnMousePressed(args);

		}

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

		MouseReleasedEventArgs args(button, _mouse_state.DisplayPosition(), _mouse_state.Position());

		for (auto i = _mouse_listeners.begin(); i != _mouse_listeners.end(); ++i)
			(*i)->OnMouseReleased(args);

	}
	void EventManager::OnMouseAxes(Event& ev) {

		ALLEGRO_MOUSE_EVENT& mouse = ev.AlPtr()->mouse;

		// Update the mouse state.
		// Only the mouse' display position will be set here; MouseState will automatically update the world position.

		_mouse_state.SetDisplayPosition(mouse.x, mouse.y);

		if (mouse.dw != 0 || mouse.dz != 0)
			_mouse_state.SetScrollState(mouse.dz < 0, mouse.dz > 0);

		// Notify all mouse listeners.

		MouseMoveEventArgs args(_mouse_state.DisplayPosition(), _mouse_state.Position());

		for (auto i = _mouse_listeners.begin(); i != _mouse_listeners.end(); ++i)
			(*i)->OnMouseMove(args);

		if (mouse.dw != 0 || mouse.dz != 0) {

			MouseScrollEventArgs args(_mouse_state.DisplayPosition(), _mouse_state.Position(), PointI(mouse.w, mouse.z), mouse.dw, mouse.dz);

			for (auto i = _mouse_listeners.begin(); i != _mouse_listeners.end(); ++i)
				(*i)->OnMouseScroll(args);

		}

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
		Display display(ev._ev.display.source);

		// Store the old size so that we can include it in the event args.
		//SizeI old_size = display.Size();

		// Acknowledge the resize.
		al_acknowledge_resize(ev._ev.display.source);

		// Notify all listeners.

		IDisplayListener::DisplaySizeChangedEventArgs args(&display);

		for (auto i = _display_listeners.begin(); i != _display_listeners.end(); ++i)
			(*i)->OnDisplaySizeChanged(args);

	}
	void EventManager::OnDisplaySwitchOut(Event& ev) {

		// Create a non-owning display for us to work with.
		Display display(ev._ev.display.source);

		// Reset mouse/keyboard IO so it's back to the default state when the display is switched back in.

		_keyboard_state.ClearKeyStates();
		_mouse_state.ClearButtonStates();

		// Notify all listeners.

		KeyboardLostEventArgs keyboard_args;

		for (auto i = _keyboard_listeners.begin(); i != _keyboard_listeners.end(); ++i)
			(*i)->OnKeyboardLost(keyboard_args);

		IDisplayListener::DisplayLostEventArgs display_args(&display);

		for (auto i = _display_listeners.begin(); i != _display_listeners.end(); ++i)
			(*i)->OnDisplayLost(display_args);

	}
	void EventManager::OnDisplaySwitchIn(Event& ev) {

		// Create a non-owning display for us to work with.
		Display display(ev._ev.display.source);

		// Notify all listeners.

		KeyboardFoundEventArgs keyboard_args;

		for (auto i = _keyboard_listeners.begin(); i != _keyboard_listeners.end(); ++i)
			(*i)->OnKeyboardFound(keyboard_args);

		IDisplayListener::DisplayFoundEventArgs display_args(&display);

		for (auto i = _display_listeners.begin(); i != _display_listeners.end(); ++i)
			(*i)->OnDisplayFound(display_args);

	}

	// Private methods

	void EventManager::_doGlobalMouseTracking() {

		if (_global_mouse_tracking_enabled && _global_mouse_tracking_required) {

			// Update the mouse position according to its global position outside of the display.

			int x, y;
			al_get_mouse_cursor_position(&x, &y);

			_mouse_state.SetDisplayPosition(x, y);

			// Notify all mouse listeners.

			MouseMoveEventArgs args(_mouse_state.DisplayPosition(), _mouse_state.Position());

			for (auto i = _mouse_listeners.begin(); i != _mouse_listeners.end(); ++i)
				(*i)->OnMouseMove(args);

		}

	}

}