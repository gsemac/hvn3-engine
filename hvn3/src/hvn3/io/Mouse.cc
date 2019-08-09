#include "hvn3/core/Engine.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/io/Display.h"

#include <allegro5/allegro.h>
#include <limits>

namespace hvn3 {

	float Mouse::x = 0.0f;
	float Mouse::y = 0.0f;
	Mouse::MouseButton Mouse::_left = Mouse::MouseButton();
	Mouse::MouseButton Mouse::_middle = Mouse::MouseButton();
	Mouse::MouseButton Mouse::_right = Mouse::MouseButton();
	bool Mouse::_scrolled_down = false;
	bool Mouse::_scrolled_up = false;
	bool Mouse::_scrolled_left = false;
	bool Mouse::_scrolled_right = false;
	PointF Mouse::_last_click_position = PointF(-1.0f, -1.0f);
	PointF Mouse::_display_position = PointF(Mouse::x, Mouse::y);

	bool Mouse::ButtonDown(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.held)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.held)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.held)
			return true;

		return false;

	}
	bool Mouse::ButtonPressed(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.pressed)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.pressed)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.pressed)
			return true;

		return false;

	}
	bool Mouse::ButtonDoubleClicked(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.dbl_clicked)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.dbl_clicked)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.dbl_clicked)
			return true;

		return false;

	}
	bool Mouse::ButtonReleased(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.released)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.released)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.released)
			return true;

		return false;

	}
	bool Mouse::ScrolledDown() {

		return _scrolled_down;

	}
	bool Mouse::ScrolledUp() {

		return _scrolled_up;

	}
	bool Mouse::ScrolledLeft() {

		return _scrolled_left;

	}
	bool Mouse::ScrolledRight() {

		return _scrolled_right;

	}
	bool Mouse::InRegion(const RectangleF& region) {

		return InRegion(region.X(), region.Y(), region.X() + region.Width(), region.Y() + region.Height());

	}
	bool Mouse::InRegion(float x1, float y1, float x2, float y2) {

		return Mouse::x >= x1 && Mouse::x < x2 && Mouse::y >= y1 && Mouse::y < y2;

	}
	PointF Mouse::Position() {

		return PointF(x, y);

	}
	PointF Mouse::GlobalPosition() {

		int x, y;
		al_get_mouse_cursor_position(&x, &y);

		return PointF(x, y);

	}
	const Point2d<float>& Mouse::DisplayPosition() {

		return _display_position;

	}
	void Mouse::ShowCursor() {

		al_show_mouse_cursor(Display::ActiveDisplay()->get());

	}
	void Mouse::HideCursor() {

		al_hide_mouse_cursor(Display::ActiveDisplay()->get());

	}
	void Mouse::SetCursor(SystemCursor cursor) {

		al_set_system_mouse_cursor(Display::ActiveDisplay()->get(), (ALLEGRO_SYSTEM_MOUSE_CURSOR)cursor);

	}
	EventSource Mouse::GetEventSource() {

		static class ComponentLifetimeHelper {
		public:
			ComponentLifetimeHelper() { Engine::RequireComponent(EngineComponent::Core | EngineComponent::IO); }
			~ComponentLifetimeHelper() { Engine::ReleaseComponent(EngineComponent::Core | EngineComponent::IO); }
		} helper;

		return al_get_mouse_event_source();

	}

	Mouse::Mouse() {}
	Mouse::MouseButton* Mouse::ToMouseButton(hvn3::MouseButton button) {

		switch (button) {
		case hvn3::MouseButton::Left:
			return &_left;
		case hvn3::MouseButton::Right:
			return &_right;
		default:
			return &_middle;
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

}