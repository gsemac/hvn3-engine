#pragma once
#include "Positionable2dBase.h"

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

	typedef Circle<float> CircleF;
	typedef Circle<int> CircleI;

}