#pragma once
#include "Vector2d.h"

namespace hvn3 {

	enum DIRECTION {
		DIRECTION_NONE = -2,
		DIRECTION_ANY = -1,
		DIRECTION_RIGHT = 0,
		DIRECTION_UP_RIGHT = 45,
		DIRECTION_UP = 90,
		DIRECTION_UP_LEFT = 135,
		DIRECTION_LEFT = 180,
		DIRECTION_DOWN_LEFT = 225,
		DIRECTION_DOWN = 270,
		DIRECTION_DOWN_RIGHT = 315
	};

	class Direction8 {

	public:
		Direction8();
		Direction8(DIRECTION direction);
		Direction8(const Vector2d& vector);

		bool IsFacingLeft() const;
		bool IsFacingRight() const;
		bool IsFacingUp() const;
		bool IsFacingDown() const;
		bool IsVertical() const;
		bool IsHorizontal() const;
		bool IsDiagonal() const;
		DIRECTION Value() const;

		bool operator==(const DIRECTION& other);
		Direction8& operator=(const DIRECTION& other);
		Direction8& operator+=(const DIRECTION& other);

	private:
		DIRECTION _direction;

	};

}