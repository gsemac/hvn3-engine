#include "hvn3/core/CoreDefs.h"
#include "hvn3/utility/Algorithm.h"

namespace hvn3 {

	void BresenhamLineAlgorithm(float x1, float y1, float x2, float y2, const std::function<bool(int, int)>& plot) {

		// Implement as iterator instead of using a callback?

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

	void SupercoverLineAlgorithm(float x1, float y1, float x2, float y2, const SizeF& cell_size, const std::function<bool(int, int)>& plot) {

		// Helpful discussion and algorithm here:
		// https://theshoemaker.de/2016/02/ray-casting-in-2d-grids/

		// The idea is to get the closet cell border insertion, plot the index of the cell it belongs to, and repeat.

		// #todo This can probably be improved quite a bit (lots of floating point computations going on here).

		LineF line(x1, y1, x2, y2);

		// Nothing to do if the line has a length of 0.
		if (line.LengthSquared() <= 0.0f)
			return;

		float t = 0.0f;
		float curX = x1;
		float curY = y1;
		float dir_x = line.DeltaX() > 0 ? 1.0f : 0.0f;
		float dir_y = line.DeltaY() > 0 ? 1.0f : 0.0f;

		while (curX >= Math::Min(x1, x2) && curX <= Math::Max(x1, x2) && curY >= Math::Min(y1, y2) && curY <= Math::Max(y1, y2)) {

			float tile_x = Math::Floor(curX / cell_size.width);
			float tile_y = Math::Floor(curY / cell_size.height);

			if (plot(static_cast<int>(tile_x), static_cast<int>(tile_y)) == hvn3::BREAK)
				break;

			float dt_x = ((tile_x + dir_x) * cell_size.width - curX) / line.DeltaX();
			float dt_y = ((tile_y + dir_y) * cell_size.height - curY) / line.DeltaY();

			if (dt_x < dt_y)
				t += dt_x + Math::Epsilon<float>::value;
			else
				t += dt_y + Math::Epsilon<float>::value;

			curX = x1 + line.DeltaX() * t;
			curY = y1 + line.DeltaY() * t;

		}

	}
	void SupercoverLineAlgorithm(const LineF& line, const SizeF& cell_size, const std::function<bool(int, int)>& plot) {
		SupercoverLineAlgorithm(line.X(), line.Y(), line.X2(), line.Y2(), cell_size, plot);
	}
	void SupercoverLineAlgorithm(const PointF& begin, const PointF& end, const SizeF& cell_size, const std::function<bool(int, int)>& plot) {
		SupercoverLineAlgorithm(begin.x, begin.y, end.x, end.y, cell_size, plot);
	}

}