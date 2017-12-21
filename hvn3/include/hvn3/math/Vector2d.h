#pragma once
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	class Vector2d {

	public:
		Vector2d();
		Vector2d(float x, float y);
		Vector2d(const PointF& start, const PointF& end);

		static Vector2d FromDirection(float degrees, float length);
		
		void SetX(float value);
		void SetY(float value);
		void SetLength(float value);
		void SetDirection(float degrees);

		// Returns the direction of the vector, from 0 to 360 degrees.
		float Direction() const;
		// Returns the angle of the vector relative to the quadrant that it is in, from 0 to 90 degrees.
		float Angle() const;
		int Quadrant() const;
		float X() const;
		float Y() const;
		float Length() const;

		float DotProduct(const Vector2d& other) const;
		Vector2d CrossProduct(const Vector2d& other) const;
		float Determinant(const Vector2d& other) const;
		Vector2d Normalize() const;

		Vector2d& operator+=(const Vector2d& other);
		Vector2d& operator-=(const Vector2d& other);
		Vector2d& operator*=(const Vector2d& other);
		Vector2d& operator/=(const Vector2d& other);
		Vector2d& operator*=(const float other);
		Vector2d& operator/=(const float other);

	private:
		float _m;
		float _x;
		float _y;

	};

	Vector2d operator+(const Vector2d& lhs, const Vector2d& rhs);
	Vector2d operator-(const Vector2d& lhs, const Vector2d& rhs);
	Vector2d operator*(const Vector2d& lhs, const Vector2d& rhs);
	Vector2d operator/(const Vector2d& lhs, const Vector2d& rhs);
	Vector2d operator*(const Vector2d& lhs, const Vector2d& rhs);
	Vector2d operator*(const Vector2d& lhs, const float rhs);
	Vector2d operator*(const float lhs, const Vector2d& rhs);
	Vector2d operator/(const Vector2d& lhs, const float rhs);
	Vector2d operator/(const float lhs, const Vector2d& rhs);
	Vector2d operator-(const Vector2d& rhs);
	
	PointF operator+(const PointF& lhs, const Vector2d& rhs);
	PointF& operator+=(PointF& lhs, const Vector2d& rhs);
	PointF operator-(const PointF& lhs, const Vector2d& rhs);
	PointF& operator-=(PointF& lhs, const Vector2d& rhs);

	std::ostream& operator<< (std::ostream& stream, const Vector2d& vector);

}