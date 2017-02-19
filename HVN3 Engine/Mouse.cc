#include <allegro5/allegro.h>
#include <limits>
#include "Mouse.h"
#include "Display.h"

#define DBL_CLICK_SEC 0.5f

float Mouse::X = -std::numeric_limits<float>::max();
float Mouse::Y = -std::numeric_limits<float>::max();
Mouse::MouseButton Mouse::__left = Mouse::MouseButton();
Mouse::MouseButton Mouse::__middle = Mouse::MouseButton();
Mouse::MouseButton Mouse::__right = Mouse::MouseButton();
bool Mouse::__scrolled_down = false;
bool Mouse::__scrolled_up = false;
Point Mouse::__last_click_pos = Point(-1.0f, -1.0f);
Point Mouse::__display_mouse_position = Point(Mouse::X, Mouse::Y);

bool Mouse::ButtonDown(MOUSE_BUTTONS mouse_button) {

	if (mouse_button & MB_LEFT && __left.held)
		return true;
	if (mouse_button & MB_RIGHT && __right.held)
		return true;
	if (mouse_button & MB_MIDDLE && __middle.held)
		return true;

	return false;

}
bool Mouse::ButtonPressed(MOUSE_BUTTONS mouse_button) {

	if (mouse_button & MB_LEFT && __left.pressed)
		return true;
	if (mouse_button & MB_RIGHT && __right.pressed)
		return true;
	if (mouse_button & MB_MIDDLE && __middle.pressed)
		return true;

	return false;

}
bool Mouse::ButtonDoubleClicked(MOUSE_BUTTONS mouse_button) {

	if (mouse_button & MB_LEFT && __left.dbl_clicked)
		return true;
	if (mouse_button & MB_RIGHT && __right.dbl_clicked)
		return true;
	if (mouse_button & MB_MIDDLE && __middle.dbl_clicked)
		return true;

	return false;

}
bool Mouse::ButtonReleased(MOUSE_BUTTONS mouse_button) {

	if (mouse_button & MB_LEFT && __left.released)
		return true;
	if (mouse_button & MB_RIGHT && __right.released)
		return true;
	if (mouse_button & MB_MIDDLE && __middle.released)
		return true;

	return false;

}
bool Mouse::ScrolledDown() {

	return __scrolled_down;

}
bool Mouse::ScrolledUp() {

	return __scrolled_up;

}
bool Mouse::InRegion(Rectangle rect) {

	return InRegion(rect.X(), rect.Y(), rect.X() + rect.Width(), rect.Y() + rect.Height());

}
bool Mouse::InRegion(float x1, float y1, float x2, float y2) {

	return Mouse::X >= x1 && Mouse::X < x2 && Mouse::Y >= y1 && Mouse::Y < y2;

}
Point Mouse::Position() {

	return Point(X, Y);

}
const Point& Mouse::DisplayPosition() {

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
::EventSource Mouse::EventSource() {

	return ::EventSource(al_get_mouse_event_source());

}

Mouse::Mouse() {}
Mouse::MouseButton* Mouse::ToMouseButton(MOUSE_BUTTONS button) {

	switch (button) {
	case MOUSE_BUTTONS::MB_LEFT: return &__left;
	case MOUSE_BUTTONS::MB_MIDDLE: return &__middle;
	default: return &__right;
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

void Mouse::StateAccessor::SetButtonState(MOUSE_BUTTONS button, bool pressed) {

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
void Mouse::StateAccessor::ResetButtonStates(bool pressed, bool released, bool held) {

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
void Mouse::StateAccessor::SetPosition(float x, float y) {

	X = x;
	Y = y;

}
void Mouse::StateAccessor::SetDisplayPosition(int x, int y) {

	__display_mouse_position.SetXY(x, y);

}
void Mouse::StateAccessor::SetScrollState(bool scrolled_up, bool scrolled_down) {

	__scrolled_down = scrolled_down;
	__scrolled_up = scrolled_up;

}