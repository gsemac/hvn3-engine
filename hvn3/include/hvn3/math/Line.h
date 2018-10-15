#pragma once
#include "hvn3/math/MathUtils.h"
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	template <typename T>
	struct LineSlopeInterceptForm {
		T slope;
		T intercept;
	};

	template <typename T>
	struct LineStandardForm {
		T A;
		T B;
		T C;
	};

	template <typename T>
	struct Line {

	public:
		Line() :
			Line(0, 0, 0, 0) {
		}
		Line(T x1, T y1, T x2, T y2) :
			_a(x1, y1),
			_b(x2, y2) {
		}
		Line(const Point2d<T>& a, const Point2d<T>& b) :
			_a(a),
			_b(b) {
		}

		const Point2d<T>& First() const {

			return _a;

		}
		const Point2d<T>& Second() const {

			return _b;

		}
		T X() const {
			return _a.x;
		}
		T Y() const {
			return _a.y;
		}
		T X2() const {
			return _b.x;
		}
		T Y2() const {
			return _b.y;
		}

		bool ContainsPoint(const Point2d<T>& point) const {

			T test_len = Line<T>(First(), point).Length() + Line<T>(point, Second()).Length();

			if (Math::IsZero((std::abs)(Length() - test_len)))
				return true;

			return false;

		}
		bool IsSteep() const {
			return (std::abs)(DeltaY()) > (std::abs)(DeltaX());
		}

		LineSlopeInterceptForm<T> GetSlopeInterceptForm() const {

			LineSlopeInterceptForm<T> out;

			out.slope = Slope();
			out.intercept = YIntercept();

			return out;

		}
		LineStandardForm<T> GetStandardForm() const {

			LineStandardForm<T> out;

			out.A = Y2() - Y();
			out.B = X() - X2();
			out.C = out.A * X() + out.B * Y();

			return out;

		}

		Point2d<T> Midpoint() const {

			return Point2d<T>((_a.X() + _b.X()) / T(2), (_a.Y() + _b.Y()) / T(2));

		}
		T Slope() const {

			return (_b.y - _a.y) / (_b.x - _a.x);

		}
		T XIntercept() const {

			return -YIntercept() / Slope();

		}
		T YIntercept() const {

			return _a.y - (Slope() * _a.x);

		}
		T DeltaX() const {
			return _b.x - _a.x;
		}
		T DeltaY() const {
			return _b.y - _a.y;
		}

		T Length() const {
			return std::sqrt(LengthSquared());
		}
		T LengthSquared() const {

			float dx = _b.X() - _a.X();
			float dy = _b.Y() - _a.Y();
			float d_sq = dx * dx + dy * dy;

			return d_sq;

		}

	private:
		Point2d<T> _a;
		Point2d<T> _b;

	};

	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const Line<T>& line) {

		if (line.DeltaX() == 0.0f)
			stream << "x = " << line.X();
		else if (line.DeltaY() == 0.0f)
			stream << "y = " << line.Y();
		else
			stream << "y = " << line.Slope() << "x + " << line.YIntercept();

		return stream;

	}
	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const LineSlopeInterceptForm<T>& line) {

		stream << "y = " << line.slope << "x + " << line.intercept;

		return stream;

	}
	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const LineStandardForm<T>& line) {

		stream << line.A << "x + " << line.B << "y = " << line.C;

		return stream;

	}

	typedef Line<float> LineF;
	typedef Line<int> LineI;

}