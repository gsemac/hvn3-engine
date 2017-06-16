#include <cmath>
#include <iostream>
#include "Vector2d.h"
#include "Geometry.h"
#include "Utility.h"
#include "Exception.h"

namespace hvn3 {

	Direction::Direction() {

		__direction = NONE;

	}
	Direction::Direction(DIRECTION direction) {

		__direction = direction;

	}

	bool Direction::FacingLeft() {

		return __direction == UP_LEFT || __direction == LEFT || __direction == DOWN_LEFT;

	}
	bool Direction::FacingRight() {

		return __direction == UP_RIGHT || __direction == RIGHT || __direction == DOWN_RIGHT;

	}
	bool Direction::FacingUp() {

		return __direction == UP_RIGHT || __direction == UP || __direction == UP_LEFT;

	}
	bool Direction::FacingDown() {

		return __direction == DOWN_RIGHT || __direction == DOWN || __direction == DOWN_LEFT;

	}
	bool Direction::IsVertical() {

		return __direction == UP || __direction == DOWN;

	}
	bool Direction::IsHorizontal() {

		return __direction == LEFT || __direction == RIGHT;

	}
	bool Direction::IsDiagonal() {

		return __direction == UP_RIGHT || __direction == UP_LEFT || __direction == DOWN_RIGHT || __direction == DOWN_LEFT;

	}
	DIRECTION Direction::Value() {

		return __direction;

	}

	bool Direction::operator==(const DIRECTION& other) {

		return __direction == other;

	}
	Direction& Direction::operator=(const DIRECTION& other) {

		__direction = other;

		return *this;

	}
	Direction& Direction::operator+=(const DIRECTION& other) {

		switch (other) {
		case DOWN:
			switch (__direction) {
			case UP: __direction = NONE;  break;
			case LEFT: __direction = DOWN_LEFT; break;
			case RIGHT: __direction = DOWN_RIGHT; break;
			case UP_LEFT: __direction = LEFT; break;
			case UP_RIGHT: __direction = RIGHT; break;
			case DOWN_LEFT: __direction = DOWN_LEFT; break;
			case DOWN_RIGHT: __direction = DOWN_RIGHT; break;
			default: __direction = DOWN; break;
			} break;
		case UP:
			switch (__direction) {
			case DOWN: __direction = NONE;  break;
			case LEFT: __direction = UP_LEFT; break;
			case RIGHT: __direction = UP_RIGHT; break;
			case UP_LEFT: __direction = UP_LEFT; break;
			case UP_RIGHT: __direction = UP_RIGHT; break;
			case DOWN_LEFT: __direction = LEFT; break;
			case DOWN_RIGHT: __direction = RIGHT; break;
			default: __direction = UP; break;
			} break;
		case LEFT:
			switch (__direction) {
			case RIGHT: __direction = NONE;  break;
			case UP: __direction = UP_LEFT; break;
			case DOWN: __direction = DOWN_LEFT; break;
			case UP_LEFT: __direction = UP_LEFT; break;
			case UP_RIGHT: __direction = UP; break;
			case DOWN_LEFT: __direction = DOWN_LEFT; break;
			case DOWN_RIGHT: __direction = DOWN; break;
			default: __direction = LEFT; break;
			} break;
		case RIGHT:
			switch (__direction) {
			case RIGHT: __direction = NONE;  break;
			case UP: __direction = UP_LEFT; break;
			case DOWN: __direction = DOWN_LEFT; break;
			case UP_LEFT: __direction = UP_LEFT; break;
			case UP_RIGHT: __direction = UP; break;
			case DOWN_LEFT: __direction = DOWN_LEFT; break;
			case DOWN_RIGHT: __direction = DOWN; break;
			default: __direction = LEFT; break;
			} break;
		}

		return *this;

	}

	Vector2d::Vector2d() :
		Vector2d(0.0f, 0.0f) {
	}
	Vector2d::Vector2d(const PointF& start, const PointF& end) {

		_x = end.X() - start.X();
		_y = end.Y() - start.Y();
		_m = std::hypotf(_x, _y);
		
	}
	Vector2d::Vector2d(float degrees, float magnitude) {

		float rad = DegreesToRadians(degrees);

		_m = magnitude;
		_x = std::cos(rad) * _m;
		_y = std::sin(rad) * -_m;


	}
	Vector2d::Vector2d(const std::pair<float, float>& components) {

		_x = components.first;
		_y = components.second;
		_m = std::hypotf(_x, _y);

	}

	float Vector2d::X() const {

		return _x;

	}
	float Vector2d::Y() const {

		return _y;

	}
	float Vector2d::Magnitude() const {

		return _m;

	}
	void Vector2d::SetX(float value) {

		_x = value;
		_m = std::hypotf(_x, _y);

	}
	void Vector2d::SetY(float value) {

		_y = value;
		_m = std::hypotf(_x, _y);

	}
	void Vector2d::SetMagnitude(float value) {

		float rad = DegreesToRadians(Angle());

		_m = value;
		_x = std::cos(rad) * _m;
		_y = std::sin(rad) * -_m;

	}
	void Vector2d::SetDirection(float degrees) {

		throw NotImplementedException();

	}

	Direction Vector2d::Direction() const {

		if (_y < 0.0f) {
			if (_x < 0.0f)
				return hvn3::Direction(UP_LEFT);
			else if (_x > 0.0f)
				return hvn3::Direction(UP_RIGHT);
			else
				return hvn3::Direction(UP);
		}
		else if (_y > 0.0f) {
			if (_x < 0.0f)
				return hvn3::Direction(DOWN_LEFT);
			else if (_x > 0.0f)
				return hvn3::Direction(DOWN_RIGHT);
			else
				return hvn3::Direction(DOWN);
		}
		else {
			if (_x < 0.0f)
				return hvn3::Direction(LEFT);
			else if (_x > 0.0f)
				return hvn3::Direction(RIGHT);
			else
				return hvn3::Direction(NONE);
		}

	}
	float Vector2d::Angle() const {

		// Initialize variables.
		float degrees = 0.0f;

		// Determine the angle using an appropriate means.
		if ((std::abs)(_x) > 0.0f)
			degrees = RadiansToDegrees((std::acos)(_x / _m));
		else if ((std::abs)(-_y) > 0.0f) {
			if (_y > 0.0f)
				degrees = 270.0f;
			else
				degrees = RadiansToDegrees((std::asin)((std::abs)(-_y) / _m));
		}

		// Adjust the angle according to its quadrant.
		switch (Quadrant()) {
		case 3: degrees += 90.0f; break;
		case 4: degrees += 270.0f; break;
		}

		// Return the result.
		return degrees;

	}
	int Vector2d::Quadrant() const {

		if (_x > 0.0f && -_y > 0.0f)
			return 1;
		else if (_x < 0.0f && -_y > 0.0f)
			return 2;
		else if (_x < 0.0f && -_y < 0.0f)
			return 3;
		else if (_x > 0.0f && -_y < 0.0f)
			return 4;

		return -1;

	}

	float Vector2d::DotProduct(const Vector2d& other) const {

		return (_x * _y) + (other._x * other._y);

	}
	Vector2d Vector2d::CrossProduct(const Vector2d& other) const {

		return Vector2d({ X() * other.X() , Y() * other.Y() });

	}

	Vector2d Vector2d::operator+(const Vector2d& other) {

		return Vector2d({ X() + other.X() , Y() + other.Y() });

	}
	Vector2d& Vector2d::operator+=(const Vector2d& other) {

		SetX(X() + other.X());
		SetY(Y() + other.Y());
		return *this;

	}
	Vector2d Vector2d::operator-(const Vector2d& other) {

		return Vector2d({ X() - other.X() , Y() - other.Y() });

	}
	Vector2d& Vector2d::operator-=(const Vector2d& other) {

		SetX(X() - other.X());
		SetY(Y() - other.Y());
		return *this;

	}
	Vector2d Vector2d::operator*(const Vector2d& other) {

		return CrossProduct(other);

	}
	Vector2d& Vector2d::operator*=(const Vector2d& other) {

		SetX(X() * other.X());
		SetY(Y() * other.Y());
		return *this;

	}
	Vector2d Vector2d::operator*(const float other) {

		return Vector2d(X() * other, Y() * other);

	}
	Vector2d& Vector2d::operator*=(const float other) {

		SetX(X() * other);
		SetY(Y() * other);

		return *this;

	}
	Vector2d Vector2d::operator-() const {

		return Vector2d(-X(), -Y());

	}

	PointF operator+(const PointF& lhs, const Vector2d& rhs) {

		return PointF(lhs.X() + rhs.X(), lhs.Y() + rhs.Y());

	}
	PointF& operator+=(PointF& lhs, const Vector2d& rhs) {

		lhs.Offset(rhs.X(), rhs.Y());

		return lhs;

	}
	PointF operator-(const PointF& lhs, const Vector2d& rhs) {

		return PointF(lhs.X() - rhs.X(), lhs.Y() - rhs.Y());

	}
	PointF& operator-=(PointF& lhs, const Vector2d& rhs) {

		lhs.Offset(-rhs.X(), -rhs.Y());

		return lhs;

	}

}