#include "hvn3/core/CoreDefs.h"
#include "hvn3/utility/Algorithm.h"

namespace hvn3 {

	void BresenhamLineAlgorithm(float x1, float y1, float x2, float y2, const std::function<bool(int, int)>& plot) {

		bool swap = false;

		// Because the loop uses x, if x doesn't vary, the loop won't execute even when y does vary.
		// As well, if the line is steep (change in y is greater than the change in x), we need use y for the loop instead to get all points.
		// To solve this, swap the components, and plot (x, y) as (y, x) instead.

		if (std::abs(x2 - x1) < 1.0f || (std::abs(y2 - y1) > std::abs(x2 - x1))) {

			std::swap(x1, y1);
			std::swap(x2, y2);

			swap = true;

		}

		float delta_x = x2 - x1;
		float delta_y = y2 - y1;
		float delta_err = std::abs(delta_y / delta_x);
		float err = 0.0f;

		int y = static_cast<int>(std::round(y1));

		// Notice that the loop goes in reverse by decrementing x if x2 < x1.
		// Some implementations would instead trace the line in the opposite direction, which plots the same points, but in the opposite order.
		// This approach is used instead to keep the points in the correct order.

		int x_inc = x2 < x1 ? -1 : 1;

		for (int x = static_cast<int>(x1); x_inc > 0 ? (x <= x2) : (x >= x2); x += x_inc) {

			bool user_requested_break = false;

			if (swap)
				user_requested_break = (plot(y, x) == hvn3::BREAK);
			else
				user_requested_break = (plot(x, y) == hvn3::BREAK);

			if (user_requested_break)
				break;

			err += delta_err;

			if (err >= 0.5f) {

				y += delta_y < 0.0f ? -1 : 1;
				err -= 1.0f;

			}

		}

	}
	void BresenhamLineAlgorithm(const LineF& line, const std::function<bool(int, int)>& plot) {
		BresenhamLineAlgorithm(line.X(), line.Y(), line.X2(), line.Y2(), plot);
	}
	void BresenhamLineAlgorithm(const PointF& begin, const PointF& end, const std::function<bool(int, int)>& plot) {
		BresenhamLineAlgorithm(begin.x, begin.y, end.x, end.y, plot);
	}

}