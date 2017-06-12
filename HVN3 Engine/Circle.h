#pragma once
#include "IPositionable2d.h"

namespace hvn3 {

	template <typename T>
	struct Circle : public IPositionable2d<T> {

	public:
		Circle(T radius) :
			Circle(0, 0, radius) {}
		Circle(T x, T y, T radius) :
			IPositionable2d(x, y) {

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

}