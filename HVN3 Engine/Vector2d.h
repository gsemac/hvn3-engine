#pragma once
#include "Point2d.h"

namespace hvn3 {

	class Vector2d {

	public:
		Vector2d();
		Vector2d(float x, float y);
		Vector2d(const PointF& start, const PointF& end);
		Vector2d(const std::pair<float, float>& components);

		static Vector2d FromDirection(float degrees, float magnitude);
		
		void SetX(float value);
		void SetY(float value);
		void SetMagnitude(float value);
		void SetDirection(float degrees);

		float Angle() const;
		int Quadrant() const;
		float X() const;
		float Y() const;
		float Magnitude() const;

		float DotProduct(const Vector2d& other) const;
		Vector2d CrossProduct(const Vector2d& other) const;

		Vector2d operator+(const Vector2d& other);
		Vector2d& operator+=(const Vector2d& other);
		Vector2d operator-(const Vector2d& other);
		Vector2d& operator-=(const Vector2d& other);
		Vector2d operator*(const Vector2d& other);
		Vector2d& operator*=(const Vector2d& other);
		Vector2d operator*(const float other);
		Vector2d& operator*=(const float other);
		Vector2d operator-() const;

	private:
		float _m;
		float _x;
		float _y;

	};

	PointF operator+(const PointF& lhs, const Vector2d& rhs);
	PointF& operator+=(PointF& lhs, const Vector2d& rhs);
	PointF operator-(const PointF& lhs, const Vector2d& rhs);
	PointF& operator-=(PointF& lhs, const Vector2d& rhs);

}