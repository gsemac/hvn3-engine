#include "hvn3/io/MouseMutator.h"
#include "hvn3/io/MouseListener.h"
#include <allegro5/allegro.h>
#define DBL_CLICK_SEC 0.5f

namespace hvn3 {
	namespace System {

		void MouseMutator::SetButtonState(hvn3::MouseButton button, bool pressed) const {

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
		void MouseMutator::MouseMutator::ResetButtonStates(bool pressed, bool released, bool held) const {

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
		void MouseMutator::MouseMutator::SetPosition(float x, float y) const {

			Mouse::x = x;
			Mouse::y = y;

		}
		void MouseMutator::MouseMutator::SetDisplayPosition(int x, int y) const {

			Mouse::_display_position.SetX(x);
			Mouse::_display_position.SetY(y);

		}
		void MouseMutator::MouseMutator::SetScrollState(bool scrolled_up, bool scrolled_down) const {

			Mouse::_scrolled_down = scrolled_down;
			Mouse::_scrolled_up = scrolled_up;

		}

		void MouseMutator::DispatchAllMouseDownEvents() const {

			if (Mouse::_left.held)
				DispatchEvent(MouseDownEventArgs(MouseButton::Left));
			if (Mouse::_right.held)
				DispatchEvent(MouseDownEventArgs(MouseButton::Right));
			if (Mouse::_middle.held)
				DispatchEvent(MouseDownEventArgs(MouseButton::Middle));

		}
		void MouseMutator::DispatchEvent(MouseDownEventArgs& e) const {

			ListenerCollection<IMouseListener>::ForEach([&](IMouseListener* i) {
				i->OnMouseDown(e);
				HVN3_CONTINUE;
			});

		}
		void MouseMutator::DispatchEvent(MousePressedEventArgs& e) const {

			ListenerCollection<IMouseListener>::ForEach([&](IMouseListener* i) {
				i->OnMousePressed(e);
				HVN3_CONTINUE;
			});

		}
		void MouseMutator::DispatchEvent(MouseReleasedEventArgs& e) const {
		
			ListenerCollection<IMouseListener>::ForEach([&](IMouseListener* i) {
				i->OnMouseReleased(e);
				HVN3_CONTINUE;
			});

		}
		void MouseMutator::DispatchEvent(MouseMoveEventArgs& e) const {

			ListenerCollection<IMouseListener>::ForEach([&](IMouseListener* i) {
				i->OnMouseMove(e);
				HVN3_CONTINUE;
			});

		}
		void MouseMutator::DispatchEvent(MouseScrollEventArgs& e) const {

			ListenerCollection<IMouseListener>::ForEach([&](IMouseListener* i) {
				i->OnMouseScroll(e);
				HVN3_CONTINUE;
			});

		}

		EventSource MouseMutator::GetEventSource() const {

			return al_get_mouse_event_source();

		}

	}
}