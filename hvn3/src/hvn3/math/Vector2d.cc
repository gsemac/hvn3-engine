#include <cmath>
#include "hvn3/math/Vector2d.h"
#include "hvn3/math/MathUtils.h"

namespace hvn3 {

	Vector2d::Vector2d() :
		Vector2d(0.0f, 0.0f) {
	}
	Vector2d::Vector2d(float x, float y) {

		_x = x;
		_y = y;
		_m = std::hypotf(_x, _y);

	}
	Vector2d::Vector2d(const PointF& start, const PointF& end) {

		_x = end.X() - start.X();
		_y = end.Y() - start.Y();
		_m = std::hypotf(_x, _y);

	}

	Vector2d Vector2d::FromDirection(float degrees, float length) {

		float rad = Math::DegreesToRadians(degrees);

		Vector2d vec;

		vec._m = length;
		vec._x = std::cos(rad) * vec._m;
		vec._y = std::sin(rad) * -vec._m;

		return vec;

	}

	float Vector2d::X() const {

		return _x;

	}
	float Vector2d::Y() const {

		return _y;

	}
	float Vector2d::Length() const {

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
	void Vector2d::SetLength(float value) {

		float rad = Math::DegreesToRadians(Direction());

		_m = value;
		_x = std::cos(rad) * _m;
		_y = std::sin(rad) * -_m;

	}
	void Vector2d::SetDirection(float degrees) {

		*this = Vector2d::FromDirection(degrees, Length());

	}

	float Vector2d::Direction() const {

		float rad = std::atan2(-_y, _x);
		float deg = Math::RadiansToDegrees(rad);

		if (deg < 0.0f)
			deg += 360.0f;

		return deg;
		
	}
	float Vector2d::Angle() const {

		float rad = std::atan(-_y / _x);
		float deg = Math::RadiansToDegrees(rad);

		if (deg < 0.0f)
			deg += 90.0f;

		return deg;

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

		return (_x * other._x) + (_y * other._y);

	}
	Vector2d Vector2d::CrossProduct(const Vector2d& other) const {

		return Vector2d(X() * other.X(), Y() * other.Y());

	}
	float Vector2d::Determinant(const Vector2d& other) const {

		return X() * other.Y() + Y() * other.X();

	}
	Vector2d Vector2d::Normalize() const {

		return *this / Length();

	}

	Vector2d& Vector2d::operator+=(const Vector2d& other) {

		SetX(X() + other.X());
		SetY(Y() + other.Y());
		return *this;

	}
	Vector2d& Vector2d::operator-=(const Vector2d& other) {

		SetX(X() - other.X());
		SetY(Y() - other.Y());
		return *this;

	}
	Vector2d& Vector2d::operator*=(const Vector2d& other) {

		SetX(X() * other.X());
		SetY(Y() * other.Y());
		return *this;

	}
	Vector2d& Vector2d::operator/=(const Vector2d& other) {

		SetX(X() / other.X());
		SetY(Y() / other.Y());
		return *this;

	}
	Vector2d& Vector2d::operator*=(const float other) {

		SetX(X() * other);
		SetY(Y() * other);

		return *this;

	}
	Vector2d& Vector2d::operator/=(const float other) {

		SetX(X() / other);
		SetY(Y() / other);

		return *this;

	}

	Vector2d operator+(const Vector2d& lhs, const Vector2d& rhs) {

		return Vector2d(lhs.X() + rhs.X(), lhs.Y() + rhs.Y());

	}
	Vector2d operator-(const Vector2d& lhs, const Vector2d& rhs) {

		return Vector2d(lhs.X() - rhs.X(), lhs.Y() - rhs.Y());

	}
	Vector2d operator*(const Vector2d& lhs, const Vector2d& rhs) {

		return lhs.CrossProduct(rhs);

	}
	Vector2d operator/(const Vector2d& lhs, const Vector2d& rhs) {

		return Vector2d(lhs.X() / rhs.X(), lhs.Y() * rhs.Y());

	}
	Vector2d operator*(const Vector2d& lhs, const float rhs) {

		return Vector2d(lhs.X() * rhs, lhs.Y() * rhs);

	}
	Vector2d operator*(const float lhs, const Vector2d& rhs) {

		return rhs * lhs;

	}
	Vector2d operator/(const Vector2d& lhs, const float rhs) {

		return Vector2d(lhs.X() / rhs, lhs.Y() / rhs);

	}
	Vector2d operator/(const float lhs, const Vector2d& rhs) {

		return rhs / lhs;

	}
	Vector2d operator-(const Vector2d& rhs) {

		return Vector2d(-rhs.X(), -rhs.Y());

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

	std::ostream& operator<< (std::ostream& stream, const Vector2d& vector) {

		stream << "[" << vector.X() << ", " << vector.Y() << "]";

		return stream;

	}

}