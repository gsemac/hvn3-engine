#pragma once
#include "hvn3/core/Positionable2dBase.h"

namespace hvn3 {

	template <typename T>
	struct Circle : public Positionable2dBase<T> {

	public:
		Circle(T radius) :
			Circle(0, 0, radius) {}
		Circle(T x, T y, T radius) :
			Positionable2dBase(x, y) {

			_radius = radius;

		}
		Circle(Point2d<T> pos, T radius) : 
			Circle(pos.X(), pos.Y(), radius) {}
		T Radius() const {

			return _radius;

		}
		
	private:
		T _radius;

	};

	template <typename T>
	bool operator>(const Circle<T>& lhs, const Circle<T>& rhs) {

		return lhs.Radius() > rhs.Radius();

	}
	template <typename T>
	bool operator<(const Circle<T>& lhs, const Circle<T>& rhs) {

		return lhs.Radius() < rhs.Radius();

	}
	template <typename T>
	bool operator==(const Circle<T>& lhs, const Circle<T>& rhs) {

		return 
			lhs.X() == rhs.X() &&
			lhs.Y() == rhs.Y() &&
			lhs.Radius() == lhs.Radius();

	}
	template <typename T>
	bool operator!=(const Circle<T>& lhs, const Circle<T>& rhs) {

		return !(lhs == rhs);

	}

	typedef Circle<float> CircleF;
	typedef Circle<int> CircleI;

}