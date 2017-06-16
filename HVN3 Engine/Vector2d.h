#pragma once
#include "Point2d.h"

namespace hvn3 {

	enum DIRECTION {
		NONE = -1,
		ANY = -1,
		RIGHT = 0,
		UP_RIGHT = 45,
		UP = 90,
		UP_LEFT = 135,
		LEFT = 180,
		DOWN_LEFT = 225,
		DOWN = 270,
		DOWN_RIGHT = 315
	};

	class Direction {

	private:
		DIRECTION __direction;

	public:
		Direction();
		Direction(DIRECTION direction);

		bool FacingLeft();
		bool FacingRight();
		bool FacingUp();
		bool FacingDown();
		bool IsVertical();
		bool IsHorizontal();
		bool IsDiagonal();
		DIRECTION Value();

		bool operator==(const DIRECTION& other);
		Direction& operator=(const DIRECTION& other);
		Direction& operator+=(const DIRECTION& other);

	};

	class Vector2d {

	private:
		float _m;
		float _x;
		float _y;

	public:
		Vector2d();
		Vector2d(const PointF& start, const PointF& end);
		Vector2d(float direction, float magnitude);
		Vector2d(const std::pair<float, float>& components);

		void SetX(float value);
		void SetY(float value);
		void SetMagnitude(float value);
		void SetDirection(float degrees);

		Direction Direction() const;
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

	};

	PointF operator+(const PointF& lhs, const Vector2d& rhs);
	PointF& operator+=(PointF& lhs, const Vector2d& rhs);
	PointF operator-(const PointF& lhs, const Vector2d& rhs);
	PointF& operator-=(PointF& lhs, const Vector2d& rhs);

}