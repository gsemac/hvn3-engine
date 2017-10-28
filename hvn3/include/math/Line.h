#pragma once

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

		Point2d<T> Midpoint() const {

			return Point2d<T>((_a.X() + _b.X()) / T(2), (_a.Y() + _b.Y()) / T(2));

		}

		T Length() const {

			float dx = _b.X() - _a.X();
			float dy = _b.Y() - _a.Y();
			float d_sq = dx * dx + dy * dy;

			return std::sqrt(d_sq);

		}

	private:
		Point2d<T> _a;
		Point2d<T> _b;

	};

	typedef Line<float> LineF;
	typedef Line<int> LineI;

}