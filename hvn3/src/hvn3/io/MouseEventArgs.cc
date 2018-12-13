#include "hvn3/io/MouseEventArgs.h"

namespace hvn3 {

	MouseEventArgs::MouseEventArgs(MouseButton button, const PointF& displayPosition, const PointF& worldPosition) {

		_button = button;
		_position = worldPosition;
		_display_position = displayPosition;

	}
	MouseButton MouseEventArgs::Button() const {
		return _button;
	}
	const PointF& MouseEventArgs::Position() const {
		return _position;
	}
	float MouseEventArgs::X() const {
		return _position.x;
	}
	float MouseEventArgs::Y() const {
		return _position.y;
	}
	const PointF& MouseEventArgs::DisplayPosition() const {
		return _display_position;
	}

	MousePressedEventArgs::MousePressedEventArgs(MouseButton button, const PointF& displayPosition, const PointF& worldPosition, int clicks) :
		MouseEventArgs(button, displayPosition, worldPosition) {

		_clicks = clicks;
	}
	int MousePressedEventArgs::Clicks() const {
		return _clicks;
	}

	MouseScrollEventArgs::MouseScrollEventArgs(const PointF& displayPosition, const PointF& worldPosition, const PointI& scrollPosition, int scrollDeltaX, int scrollDeltaY) :
		MouseEventArgs(static_cast<MouseButton>(0), displayPosition, worldPosition) {

		_scroll_position = scrollPosition;
		_x_delta = scrollDeltaX;
		_y_delta = scrollDeltaY;

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
	const PointI& MouseScrollEventArgs::ScrollPosition() const {
		return _scroll_position;
	}
	int MouseScrollEventArgs::ScrollX() const {
		return _scroll_position.x;
	}
	int MouseScrollEventArgs::ScrollY() const {
		return _scroll_position.y;
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

	MouseMoveEventArgs::MouseMoveEventArgs(const PointF& displayPosition, const PointF& worldPosition) :
		MouseEventArgs(static_cast<MouseButton>(0), displayPosition, worldPosition) {
	}

}