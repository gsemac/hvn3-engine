#include <allegro5/allegro.h>
#include <limits>
#include "Mouse.h"
#include "Display.h"

#define DBL_CLICK_SEC 0.5f

namespace hvn3 {

	float Mouse::X = -std::numeric_limits<float>::max();
	float Mouse::Y = -std::numeric_limits<float>::max();
	Mouse::MouseButton Mouse::__left = Mouse::MouseButton();
	Mouse::MouseButton Mouse::__middle = Mouse::MouseButton();
	Mouse::MouseButton Mouse::__right = Mouse::MouseButton();
	bool Mouse::__scrolled_down = false;
	bool Mouse::__scrolled_up = false;
	bool Mouse::_scrolled_left = false;
	bool Mouse::_scrolled_right = false;
	PointF Mouse::__last_click_pos = PointF(-1.0f, -1.0f);
	PointF Mouse::__display_mouse_position = PointF(Mouse::X, Mouse::Y);

	bool Mouse::ButtonDown(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && __left.held)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && __right.held)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && __middle.held)
			return true;

		return false;

	}
	bool Mouse::ButtonPressed(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && __left.pressed)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && __right.pressed)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && __middle.pressed)
			return true;

		return false;

	}
	bool Mouse::ButtonDoubleClicked(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && __left.dbl_clicked)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && __right.dbl_clicked)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && __middle.dbl_clicked)
			return true;

		return false;

	}
	bool Mouse::ButtonReleased(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && __left.released)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && __right.released)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && __middle.released)
			return true;

		return false;

	}
	bool Mouse::ScrolledDown() {

		return __scrolled_down;

	}
	bool Mouse::ScrolledUp() {

		return __scrolled_up;

	}
	bool Mouse::ScrolledLeft() {

		return _scrolled_left;

	}
	bool Mouse::ScrolledRight() {

		return _scrolled_right;

	}
	bool Mouse::InRegion(Rectangle<float> rect) {

		return InRegion(rect.X(), rect.Y(), rect.X() + rect.Width(), rect.Y() + rect.Height());

	}
	bool Mouse::InRegion(float x1, float y1, float x2, float y2) {

		return Mouse::X >= x1 && Mouse::X < x2 && Mouse::Y >= y1 && Mouse::Y < y2;

	}
	PointF Mouse::Position() {

		return PointF(X, Y);

	}
	PointF Mouse::GlobalPosition() {

		int x, y;
		al_get_mouse_cursor_position(&x, &y);

		return PointF(x, y);

	}
	const Point2d<float>& Mouse::DisplayPosition() {

		return __display_mouse_position;

	}
	void Mouse::ShowCursor() {

		al_show_mouse_cursor(Display::ActiveDisplay()->AlPtr());

	}
	void Mouse::HideCursor() {

		al_hide_mouse_cursor(Display::ActiveDisplay()->AlPtr());

	}
	void Mouse::SetCursor(SystemCursor cursor) {

		al_set_system_mouse_cursor(Display::ActiveDisplay()->AlPtr(), (ALLEGRO_SYSTEM_MOUSE_CURSOR)cursor);

	}
	hvn3::EventSource Mouse::EventSource() {

		return hvn3::EventSource(al_get_mouse_event_source());

	}

	Mouse::Mouse() {}
	Mouse::MouseButton* Mouse::ToMouseButton(hvn3::MouseButton button) {

		switch (button) {
		case hvn3::MouseButton::Left:
			return &__left;
		case hvn3::MouseButton::Right:
			return &__middle;
		default:
			return &__right;
		}

	}

	Mouse::MouseButton::MouseButton() : last_release(true) {

		held = false;
		pressed = false;
		locked = false;
		released = false;

		dbl_clicked = false;
		dbl_waiting = false;
		dbl_waiting_allowed = true;

	}

	void Mouse::MouseController::SetButtonState(hvn3::MouseButton button, bool pressed) {

		Mouse::MouseButton* mb = ToMouseButton(button);

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
				__last_click_pos = Position();
			}
			else
				mb->dbl_waiting_allowed = true;
			mb->last_release.Reset(true);
		}
		else if (mb->pressed && mb->dbl_waiting) {
			// Detect double-click.
			if (mb->last_release.SecondsElapsed() < DBL_CLICK_SEC && __last_click_pos == Position())
				mb->dbl_clicked = true;
			mb->dbl_waiting = false;
			// Prevent the next release from triggering wait.
			mb->dbl_waiting_allowed = false;
			// if (mb->dbl_clicked) std::cout << "DOUBLE-CLICKED!\n";
		}

	}
	void Mouse::MouseController::ResetButtonStates(bool pressed, bool released, bool held) {

		if (pressed) {
			__left.pressed = false;
			__right.pressed = false;
			__middle.pressed = false;
		}

		if (released) {
			__left.released = false;
			__right.released = false;
			__middle.released = false;
		}

		if (held) {
			__left.held = false;
			__right.held = false;
			__middle.held = false;
		}

		__scrolled_up = false;
		__scrolled_down = false;

		__left.dbl_clicked = false;
		__right.dbl_clicked = false;
		__middle.dbl_clicked = false;

		if (__left.last_release.SecondsElapsed() > DBL_CLICK_SEC)
			__left.dbl_waiting = false;
		if (__right.last_release.SecondsElapsed() > DBL_CLICK_SEC)
			__right.dbl_waiting = false;
		if (__middle.last_release.SecondsElapsed() > DBL_CLICK_SEC)
			__middle.dbl_waiting = false;

	}
	void Mouse::MouseController::SetPosition(float x, float y) {

		X = x;
		Y = y;

	}
	void Mouse::MouseController::SetDisplayPosition(int x, int y) {

		__display_mouse_position.SetX(x);
		__display_mouse_position.SetY(y);

	}
	void Mouse::MouseController::SetScrollState(bool scrolled_up, bool scrolled_down) {

		__scrolled_down = scrolled_down;
		__scrolled_up = scrolled_up;

	}

}