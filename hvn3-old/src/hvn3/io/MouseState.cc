#include "hvn3/io/MouseState.h"

#include <allegro5/mouse.h>

constexpr float DBL_CLICK_SEC = 0.5f;

namespace hvn3 {

	// MouseState

	// Public methods

	MouseState::MouseState() :
		x(0.0f),
		y(0.0f),
		_scrolled_down(false),
		_scrolled_up(false),
		_scrolled_left(false),
		_scrolled_right(false),
		_last_click_position(-1.0f, -1.0f),
		_display_position(0.0f, 0.0f) {
	}

	bool MouseState::ButtonDown(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.held)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.held)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.held)
			return true;

		return false;

	}
	bool MouseState::ButtonPressed(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.pressed)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.pressed)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.pressed)
			return true;

		return false;

	}
	bool MouseState::ButtonDoubleClicked(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.dbl_clicked)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.dbl_clicked)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.dbl_clicked)
			return true;

		return false;

	}
	bool MouseState::ButtonReleased(hvn3::MouseButton mouse_button) {

		if ((int)mouse_button & (int)hvn3::MouseButton::Left && _left.released)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Right && _right.released)
			return true;
		if ((int)mouse_button & (int)hvn3::MouseButton::Middle && _middle.released)
			return true;

		return false;

	}
	bool MouseState::ScrolledDown() {

		return _scrolled_down;

	}
	bool MouseState::ScrolledUp() {

		return _scrolled_up;

	}
	bool MouseState::ScrolledLeft() {

		return _scrolled_left;

	}
	bool MouseState::ScrolledRight() {

		return _scrolled_right;

	}
	PointF MouseState::Position() {

		return PointF(x, y);

	}
	PointF MouseState::ScreenPosition() {

		int x, y;
		al_get_mouse_cursor_position(&x, &y);

		return PointF(x, y);

	}
	const Point2d<float>& MouseState::DisplayPosition() {

		return _display_position;

	}

	void MouseState::SetButtonState(hvn3::MouseButton button, bool pressed) {

		MouseButtonState* mb = _toMouseButtonState(button);

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
				MouseState::_last_click_position = MouseState::Position();

			}
			else
				mb->dbl_waiting_allowed = true;

			mb->last_release.Clear();

		}
		else if (mb->pressed && mb->dbl_waiting) {

			// Detect double-click.
			if (mb->last_release.SecondsElapsed() < DBL_CLICK_SEC && MouseState::_last_click_position == MouseState::Position())
				mb->dbl_clicked = true;

			mb->dbl_waiting = false;

			// Prevent the next release from triggering wait.
			mb->dbl_waiting_allowed = false;

		}

	}
	void MouseState::ClearButtonStates() {
		ClearButtonStates(true, true, true);
	}
	void MouseState::ClearButtonStates(bool pressed, bool released, bool held) {

		if (pressed) {

			_left.pressed = false;
			_right.pressed = false;
			_middle.pressed = false;

		}

		if (released) {

			_left.released = false;
			_right.released = false;
			_middle.released = false;

		}

		if (held) {

			_left.held = false;
			_right.held = false;
			_middle.held = false;

		}

		_scrolled_up = false;
		_scrolled_down = false;

		_right.dbl_clicked = false;
		_middle.dbl_clicked = false;

		if (_left.last_release.SecondsElapsed() > DBL_CLICK_SEC)
			_left.dbl_waiting = false;

		if (_right.last_release.SecondsElapsed() > DBL_CLICK_SEC)
			_right.dbl_waiting = false;

		if (_middle.last_release.SecondsElapsed() > DBL_CLICK_SEC)
			_middle.dbl_waiting = false;

	}
	void MouseState::SetPosition(float x, float y) {

		this->x = x;
		this->y = y;

	}
	void MouseState::SetDisplayPosition(int x, int y) {

		_display_position.SetX(x);
		_display_position.SetY(y);

	}
	void MouseState::SetScrollState(bool scrolledUp, bool scrolledDown) {

		_scrolled_up = scrolledUp;
		_scrolled_down = scrolledDown;

	}

	MouseState::Iterator MouseState::begin() {
		return Iterator(this, MouseButton::Left);
	}
	MouseState::Iterator MouseState::end() {
		return Iterator(this, static_cast<MouseButton>(static_cast<int>(MouseButton::Right) + 1));
	}

	// Private methods

	MouseState::MouseButtonState* MouseState::_toMouseButtonState(MouseButton button) {

		switch (button) {
		case hvn3::MouseButton::Left:
			return &_left;
		case hvn3::MouseButton::Right:
			return &_right;
		default:
			return &_middle;
		}

	}

	// MouseState::MouseButtonState

	// Public methods

	MouseState::MouseButtonState::MouseButtonState() :
		last_release(true) {

		held = false;
		pressed = false;
		locked = false;
		released = false;

		dbl_clicked = false;
		dbl_waiting = false;
		dbl_waiting_allowed = true;

	}

	// MouseState::Iterator

	// Public methods

	MouseState::Iterator::Iterator(MouseState* state, MouseButton button) :
		_state(state),
		_button(button) {
	}

	bool MouseState::Iterator::operator==(const Iterator& other) {

		return _button == other._button;

	}
	bool MouseState::Iterator::operator!=(const Iterator& other) {
		return !(*this == other);
	}
	MouseState::Iterator& MouseState::Iterator::operator++() {

		_increment();

		return *this;

	}
	MouseState::Iterator MouseState::Iterator::operator++(int) {

		Iterator before = *this;

		_increment();

		return before;

	}
	MouseState::Iterator::MouseButtonStateWrapper& MouseState::Iterator::operator*() {

		_initMouseButtonState();

		return _mouse_button_state;

	}
	MouseState::Iterator::MouseButtonStateWrapper* MouseState::Iterator::operator->() {
		return &(**this);
	}
	const MouseState::Iterator::MouseButtonStateWrapper& MouseState::Iterator::operator*() const {

		_initMouseButtonState();

		return _mouse_button_state;

	}
	const MouseState::Iterator::MouseButtonStateWrapper* MouseState::Iterator::operator->() const {
		return &(**this);
	}

	// Private methods

	void MouseState::Iterator::_initMouseButtonState() const {
		_mouse_button_state = MouseButtonStateWrapper(_button, *_state->_toMouseButtonState(_button));
	}
	void MouseState::Iterator::_increment() {

		switch (_button) {

		case MouseButton::Left:
			_button = MouseButton::Middle;
			break;

		case MouseButton::Middle:
			_button = MouseButton::Right;
			break;

		case MouseButton::Right:
			// Used to indicate the end of the range.
			_button = static_cast<MouseButton>(static_cast<int>(MouseButton::Right) + 1);
			break;

		}

	}

	// MouseState::Iterator::MouseButtonStateWrapper

	// Public methods

	MouseState::Iterator::MouseButtonStateWrapper::MouseButtonStateWrapper(MouseButton button, MouseButtonState state) :
		_button(button),
		_state(state) {
	}

	bool MouseState::Iterator::MouseButtonStateWrapper::Held() const {
		return _state.held;
	}
	bool MouseState::Iterator::MouseButtonStateWrapper::Pressed() const {
		return _state.pressed;
	}
	bool MouseState::Iterator::MouseButtonStateWrapper::Released() const {
		return _state.released;
	}
	MouseButton MouseState::Iterator::MouseButtonStateWrapper::Button() const {
		return _button;
	}

}