#include "hvn3/utility/Direction8.h"

namespace hvn3 {

	Direction8::Direction8() {

		_direction = DIRECTION_NONE;

	}
	Direction8::Direction8(DIRECTION direction) {

		_direction = direction;

	}
	Direction8::Direction8(DIRECTION horizontal, DIRECTION vertical) :
		Direction8(horizontal) {

		*this += vertical;

	}
	Direction8::Direction8(const Vector2d& vector) {

		if (vector.Y() < 0.0f) {
			if (vector.X() < 0.0f)
				_direction = DIRECTION_UP_LEFT;
			else if (vector.X() > 0.0f)
				_direction = DIRECTION_UP_RIGHT;
			else
				_direction = DIRECTION_UP;
		}
		else if (vector.Y() > 0.0f) {
			if (vector.X() < 0.0f)
				_direction = DIRECTION_DOWN_LEFT;
			else if (vector.X() > 0.0f)
				_direction = DIRECTION_DOWN_RIGHT;
			else
				_direction = DIRECTION_DOWN;
		}
		else {
			if (vector.X() < 0.0f)
				_direction = DIRECTION_LEFT;
			else if (vector.X() > 0.0f)
				_direction = DIRECTION_RIGHT;
			else
				_direction = DIRECTION_NONE;
		}

	}

	bool Direction8::IsFacingLeft() const {

		return _direction == DIRECTION_UP_LEFT || _direction == DIRECTION_LEFT || _direction == DIRECTION_DOWN_LEFT;

	}
	bool Direction8::IsFacingRight() const {

		return _direction == DIRECTION_UP_RIGHT || _direction == DIRECTION_RIGHT || _direction == DIRECTION_DOWN_RIGHT;

	}
	bool Direction8::IsFacingUp() const {

		return _direction == DIRECTION_UP_RIGHT || _direction == DIRECTION_UP || _direction == DIRECTION_UP_LEFT;

	}
	bool Direction8::IsFacingDown() const {

		return _direction == DIRECTION_DOWN_RIGHT || _direction == DIRECTION_DOWN || _direction == DIRECTION_DOWN_LEFT;

	}
	bool Direction8::IsVertical() const {

		return _direction == DIRECTION_UP || _direction == DIRECTION_DOWN;

	}
	bool Direction8::IsHorizontal() const {

		return _direction == DIRECTION_LEFT || _direction == DIRECTION_RIGHT;

	}
	bool Direction8::IsDiagonal() const {

		return _direction == DIRECTION_UP_RIGHT || _direction == DIRECTION_UP_LEFT || _direction == DIRECTION_DOWN_RIGHT || _direction == DIRECTION_DOWN_LEFT;

	}
	DIRECTION Direction8::Value() const {

		return _direction;

	}
	DIRECTION Direction8::HorizontalDirection() const {

		switch (_direction) {
		case DIRECTION_RIGHT:
		case DIRECTION_UP_RIGHT:
		case DIRECTION_DOWN_RIGHT:
			return DIRECTION_RIGHT;
		case DIRECTION_LEFT:
		case DIRECTION_UP_LEFT:
		case DIRECTION_DOWN_LEFT:
			return DIRECTION_LEFT;
		default:
			return DIRECTION_NONE;
		}

	}
	DIRECTION Direction8::VerticalDirection() const {

		switch (_direction) {
		case DIRECTION_UP:
		case DIRECTION_UP_RIGHT:
		case DIRECTION_UP_LEFT:
			return DIRECTION_UP;
		case DIRECTION_DOWN:
		case DIRECTION_DOWN_RIGHT:
		case DIRECTION_DOWN_LEFT:
			return DIRECTION_DOWN;
		default:
			return DIRECTION_NONE;
		}

	}

	bool Direction8::operator==(const DIRECTION& other) {

		return _direction == other;

	}
	Direction8& Direction8::operator=(const DIRECTION& other) {

		_direction = other;

		return *this;

	}
	Direction8& Direction8::operator+=(const DIRECTION& other) {

		switch (other) {
		case DIRECTION_DOWN:
			switch (_direction) {
			case DIRECTION_UP: _direction = DIRECTION_NONE;  break;
			case DIRECTION_LEFT: _direction = DIRECTION_DOWN_LEFT; break;
			case DIRECTION_RIGHT: _direction = DIRECTION_DOWN_RIGHT; break;
			case DIRECTION_UP_LEFT: _direction = DIRECTION_LEFT; break;
			case DIRECTION_UP_RIGHT: _direction = DIRECTION_RIGHT; break;
			case DIRECTION_DOWN_LEFT: _direction = DIRECTION_DOWN_LEFT; break;
			case DIRECTION_DOWN_RIGHT: _direction = DIRECTION_DOWN_RIGHT; break;
			default: _direction = DIRECTION_DOWN; break;
			} break;
		case DIRECTION_UP:
			switch (_direction) {
			case DIRECTION_DOWN: _direction = DIRECTION_NONE;  break;
			case DIRECTION_LEFT: _direction = DIRECTION_UP_LEFT; break;
			case DIRECTION_RIGHT: _direction = DIRECTION_UP_RIGHT; break;
			case DIRECTION_UP_LEFT: _direction = DIRECTION_UP_LEFT; break;
			case DIRECTION_UP_RIGHT: _direction = DIRECTION_UP_RIGHT; break;
			case DIRECTION_DOWN_LEFT: _direction = DIRECTION_LEFT; break;
			case DIRECTION_DOWN_RIGHT: _direction = DIRECTION_RIGHT; break;
			default: _direction = DIRECTION_UP; break;
			} break;
		case DIRECTION_LEFT:
			switch (_direction) {
			case DIRECTION_RIGHT: _direction = DIRECTION_NONE;  break;
			case DIRECTION_UP: _direction = DIRECTION_UP_LEFT; break;
			case DIRECTION_DOWN: _direction = DIRECTION_DOWN_LEFT; break;
			case DIRECTION_UP_LEFT: _direction = DIRECTION_UP_LEFT; break;
			case DIRECTION_UP_RIGHT: _direction = DIRECTION_UP; break;
			case DIRECTION_DOWN_LEFT: _direction = DIRECTION_DOWN_LEFT; break;
			case DIRECTION_DOWN_RIGHT: _direction = DIRECTION_DOWN; break;
			default: _direction = DIRECTION_LEFT; break;
			} break;
		case DIRECTION_RIGHT:
			switch (_direction) {
			case DIRECTION_RIGHT: _direction = DIRECTION_NONE;  break;
			case DIRECTION_UP: _direction = DIRECTION_UP_LEFT; break;
			case DIRECTION_DOWN: _direction = DIRECTION_DOWN_LEFT; break;
			case DIRECTION_UP_LEFT: _direction = DIRECTION_UP_LEFT; break;
			case DIRECTION_UP_RIGHT: _direction = DIRECTION_UP; break;
			case DIRECTION_DOWN_LEFT: _direction = DIRECTION_DOWN_LEFT; break;
			case DIRECTION_DOWN_RIGHT: _direction = DIRECTION_DOWN; break;
			default: _direction = DIRECTION_LEFT; break;
			} break;
		}

		return *this;

	}

}