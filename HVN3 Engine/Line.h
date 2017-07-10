#pragma once
#include "PointGeometry.h"

namespace hvn3 {

	template <typename T>
	struct Line {

	public:
		Line(T x1, T y1, T x2, T y2) :
			_a(x1, y1),
			_b(x2, y2) {}
		Line(const Point2d<T>& a, const Point2d<T>& b) :
			_a(a),
			_b(b) {}

		const Point2d<T>& First() const {

			return _a;

		}
		const Point2d<T>& Second() const {

			return _b;

		}

		T Length() const {

			return PointDistance(_a, _b);

		}

	private:
		Point2d<T> _a;
		Point2d<T> _b;

	};

	typedef Line<float> LineF;
	typedef Line<int> LineI;

}