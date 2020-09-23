#include "hvn3/io/IMouseListener.h"

namespace hvn3 {

	IMouseListener::MouseEventArgs::MouseEventArgs(MouseButton button, const PointF& displayPosition, const PointF& worldPosition) {

		_button = button;
		_position = worldPosition;
		_display_position = displayPosition;

	}
	MouseButton IMouseListener::MouseEventArgs::Button() const {
		return _button;
	}
	const PointF& IMouseListener::MouseEventArgs::Position() const {
		return _position;
	}
	float IMouseListener::MouseEventArgs::X() const {
		return _position.x;
	}
	float IMouseListener::MouseEventArgs::Y() const {
		return _position.y;
	}
	const PointF& IMouseListener::MouseEventArgs::DisplayPosition() const {
		return _display_position;
	}

	IMouseListener::MousePressedEventArgs::MousePressedEventArgs(MouseButton button, const PointF& displayPosition, const PointF& worldPosition, int clicks) :
		MouseEventArgs(button, displayPosition, worldPosition) {

		_clicks = clicks;
	}
	int IMouseListener::MousePressedEventArgs::Clicks() const {
		return _clicks;
	}

	IMouseListener::MouseScrollEventArgs::MouseScrollEventArgs(const PointF& displayPosition, const PointF& worldPosition, const PointI& scrollPosition, int scrollDeltaX, int scrollDeltaY) :
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
	const PointI& IMouseListener::MouseScrollEventArgs::ScrollPosition() const {
		return _scroll_position;
	}
	int IMouseListener::MouseScrollEventArgs::ScrollX() const {
		return _scroll_position.x;
	}
	int IMouseListener::MouseScrollEventArgs::ScrollY() const {
		return _scroll_position.y;
	}
	int IMouseListener::MouseScrollEventArgs::DeltaX() const {
		return _x_delta;
	}
	int IMouseListener::MouseScrollEventArgs::DeltaY() const {
		return _y_delta;
	}
	MouseScrollDirection IMouseListener::MouseScrollEventArgs::Direction() const {
		return _direction;
	}

	IMouseListener::MouseMoveEventArgs::MouseMoveEventArgs(const PointF& displayPosition, const PointF& worldPosition) :
		MouseEventArgs(static_cast<MouseButton>(0), displayPosition, worldPosition) {
	}

}