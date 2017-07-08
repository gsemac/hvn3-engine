#pragma once
#include "Point2d.h"

namespace hvn3 {

	template <typename T>
	class IPositionable2d {

	public:
		virtual T X() const = 0;
		virtual T Y() const = 0;

		virtual void SetX(T x) = 0;
		virtual void SetY(T y) = 0;

		virtual Point2d<T> Position() const = 0;
		virtual void SetPosition(const Point2d<T>& position) = 0;
		virtual void SetPosition(T x, T y) = 0;

	};

}