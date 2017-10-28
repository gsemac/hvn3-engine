#include "io/MouseEventArgs.h"

namespace hvn3 {

	MouseEventArgs::MouseEventArgs(MouseButton button) {

		_button = button;
		_position = Mouse::Position();

	}
	MouseButton MouseEventArgs::Button() const {

		return _button;

	}
	const PointF& MouseEventArgs::Position() const {

		return _position;

	}

	MousePressedEventArgs::MousePressedEventArgs(MouseButton button, int clicks) :
		MouseEventArgs(button) {

		_clicks = clicks;

	}
	int MousePressedEventArgs::Clicks() const {

		return _clicks;

	}

	MouseScrollEventArgs::MouseScrollEventArgs(int x, int y, int x_delta, int y_delta) {

		_x = x;
		_y = y;
		_x_delta = x_delta;
		_y_delta = y_delta;
		_position = Mouse::Position();

		_direction = (MouseScrollDirection)0;

		if (_x_delta < 0)
			_direction |= MouseScrollDirection::Left;
		else if (_x_delta > 0)
			_direction |= MouseScrollDirection::Right;
		if (_y_delta < 0)
			_direction |= MouseScrollDirection::Down;
		else if (_y_delta > 0)
			_direction |= MouseScrollDirection::Up;

	}
	int MouseScrollEventArgs::ScrollX() const {

		return _x;

	}
	int MouseScrollEventArgs::ScrollY() const {

		return _y;

	}
	int MouseScrollEventArgs::DeltaX() const {

		return _x_delta;

	}
	int MouseScrollEventArgs::DeltaY() const {

		return _y_delta;

	}
	MouseScrollDirection MouseScrollEventArgs::Direction() const {

		return _direction;

	}
	const PointF& MouseScrollEventArgs::Position() const {

		return _position;

	}

	MouseMoveEventArgs::MouseMoveEventArgs() {

		_position = Mouse::Position();

	}
	const PointF& MouseMoveEventArgs::Position() const {

		return _position;

	}
}