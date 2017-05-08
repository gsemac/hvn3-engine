#include <cmath>
#include <iostream>
#include "Vector2d.h"
#include "Geometry.h"
#include "Utility.h"

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

Vector2d::Vector2d() : Vector2d(0.0f, 0.0f) {}
Vector2d::Vector2d(float degrees, float magnitude) {

	__v = magnitude;
	float rad = DegreesToRadians(degrees);
	__v_x = std::cos(rad) * __v;
	__v_y = std::sin(rad) * -__v;


}
Vector2d::Vector2d(const std::pair<float, float>& components) {

	__v_x = components.first;
	__v_y = components.second;
	__v = std::hypotf(__v_x, __v_y);

}

float Vector2d::X() const {

	return __v_x;

}
float Vector2d::Y() const {

	return __v_y;

}
float Vector2d::Magnitude() const {

	return __v;

}
void Vector2d::SetX(float value) {

	__v_x = value;
	__v = std::hypotf(__v_x, __v_y);

}
void Vector2d::SetY(float value) {

	__v_y = value;
	__v = std::hypotf(__v_x, __v_y);

}
void Vector2d::SetMagnitude(float value) {

	float rad = DegreesToRadians(Angle());

	__v = value;
	__v_x = std::cos(rad) * __v;
	__v_y = std::sin(rad) * -__v;

}
void Vector2d::SetDirection(float degrees) {



}

Direction Vector2d::Direction() const {

	if (__v_y < 0.0f) {
		if (__v_x < 0.0f)
			return ::Direction(UP_LEFT);
		else if (__v_x > 0.0f)
			return ::Direction(UP_RIGHT);
		else
			return ::Direction(UP);
	}
	else if (__v_y > 0.0f) {
		if (__v_x < 0.0f)
			return ::Direction(DOWN_LEFT);
		else if (__v_x > 0.0f)
			return ::Direction(DOWN_RIGHT);
		else
			return ::Direction(DOWN);
	}
	else {
		if (__v_x < 0.0f)
			return ::Direction(LEFT);
		else if (__v_x > 0.0f)
			return ::Direction(RIGHT);
		else
			return ::Direction(NONE);
	}

}
float Vector2d::Angle() const {

	// Initialize variables.
	float degrees = 0.0f;

	// Determine the angle using an appropriate means.
	if ((std::abs)(__v_x) > 0.0f)
		degrees = RadiansToDegrees((std::acos)(__v_x / __v));
	else if ((std::abs)(-__v_y) > 0.0f) {
		if (__v_y > 0.0f) 
			degrees = 270.0f;
		else
			degrees = RadiansToDegrees((std::asin)((std::abs)(-__v_y) / __v));
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

	if (__v_x > 0.0f && -__v_y > 0.0f)
		return 1;
	else if (__v_x < 0.0f && -__v_y > 0.0f)
		return 2;
	else if (__v_x < 0.0f && -__v_y < 0.0f)
		return 3;
	else if (__v_x > 0.0f && -__v_y < 0.0f)
		return 4;

	return -1;

}

float Vector2d::DotProduct(const Vector2d& other) const {

	return (__v_x * __v_y) + (other.__v_x * other.__v_y);

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