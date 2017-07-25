#include "MouseController.h"
#include "MouseEventListener.h"
#include <allegro5/allegro.h>
#define DBL_CLICK_SEC 0.5f

namespace hvn3 {
	namespace System {

		void MouseController::SetButtonState(hvn3::MouseButton button, bool pressed) const {

			Mouse::MouseButton* mb = Mouse::ToMouseButton(button);

			// Assign state to the mouse button.
			mb->held = pressed;
			if (!(mb->locked))
				mb->pressed = pressed;
			mb->released = !pressed;
			mb->locked = pressed;

			// Check for double-clicks (press > release > press).
			if (mb->released) {

				if (mb->dbl_waiting_allowed) {

					// Wait for the next press.
					mb->dbl_waiting = true;
					Mouse::_last_click_position = Mouse::Position();

				}
				else
					mb->dbl_waiting_allowed = true;

				mb->last_release.Reset();

			}
			else if (mb->pressed && mb->dbl_waiting) {

				// Detect double-click.
				if (mb->last_release.SecondsElapsed() < DBL_CLICK_SEC && Mouse::_last_click_position == Mouse::Position())
					mb->dbl_clicked = true;

				mb->dbl_waiting = false;

				// Prevent the next release from triggering wait.
				mb->dbl_waiting_allowed = false;

			}

		}
		void MouseController::MouseController::ResetButtonStates(bool pressed, bool released, bool held) const {

			if (pressed) {
				Mouse::_left.pressed = false;
				Mouse::_right.pressed = false;
				Mouse::_middle.pressed = false;
			}

			if (released) {
				Mouse::_left.released = false;
				Mouse::_right.released = false;
				Mouse::_middle.released = false;
			}

			if (held) {
				Mouse::_left.held = false;
				Mouse::_right.held = false;
				Mouse::_middle.held = false;
			}

			Mouse::_scrolled_up = false;
			Mouse::_scrolled_down = false;

			Mouse::_left.dbl_clicked = false;
			Mouse::_right.dbl_clicked = false;
			Mouse::_middle.dbl_clicked = false;

			if (Mouse::_left.last_release.SecondsElapsed() > DBL_CLICK_SEC)
				Mouse::_left.dbl_waiting = false;
			if (Mouse::_right.last_release.SecondsElapsed() > DBL_CLICK_SEC)
				Mouse::_right.dbl_waiting = false;
			if (Mouse::_middle.last_release.SecondsElapsed() > DBL_CLICK_SEC)
				Mouse::_middle.dbl_waiting = false;

		}
		void MouseController::MouseController::SetPosition(float x, float y) const {

			Mouse::X = x;
			Mouse::Y = y;

		}
		void MouseController::MouseController::SetDisplayPosition(int x, int y) const {

			Mouse::_display_position.SetX(x);
			Mouse::_display_position.SetY(y);

		}
		void MouseController::MouseController::SetScrollState(bool scrolled_up, bool scrolled_down) const {

			Mouse::_scrolled_down = scrolled_down;
			Mouse::_scrolled_up = scrolled_up;

		}

		void MouseController::DispatchAllMouseDownEvents() const {

			if (Mouse::_left.held)
				DispatchEvent(MouseDownEventArgs(MouseButton::Left));
			if (Mouse::_right.held)
				DispatchEvent(MouseDownEventArgs(MouseButton::Right));
			if (Mouse::_middle.held)
				DispatchEvent(MouseDownEventArgs(MouseButton::Middle));

		}
		void MouseController::DispatchEvent(MouseDownEventArgs& e) const {

			for (auto i = MouseEventListener::_listeners.begin(); i != MouseEventListener::_listeners.end(); ++i)
				(*i)->OnMouseDown(e);

		}
		void MouseController::DispatchEvent(MousePressedEventArgs& e) const {

			for (auto i = MouseEventListener::_listeners.begin(); i != MouseEventListener::_listeners.end(); ++i)
				(*i)->OnMousePressed(e);

		}
		void MouseController::DispatchEvent(MouseUpEventArgs& e) const {

			for (auto i = MouseEventListener::_listeners.begin(); i != MouseEventListener::_listeners.end(); ++i)
				(*i)->OnMouseUp(e);

		}
		void MouseController::DispatchEvent(MouseMoveEventArgs& e) const {

			for (auto i = MouseEventListener::_listeners.begin(); i != MouseEventListener::_listeners.end(); ++i)
				(*i)->OnMouseMove(e);

		}
		void MouseController::DispatchEvent(MouseScrollEventArgs& e) const {

			for (auto i = MouseEventListener::_listeners.begin(); i != MouseEventListener::_listeners.end(); ++i)
				(*i)->OnMouseScroll(e);

		}

		System::EventSource MouseController::GetEventSource() const {

			return al_get_mouse_event_source();

		}

	}
}