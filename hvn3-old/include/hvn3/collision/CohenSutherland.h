#pragma once

#include "hvn3/math/Rectangle.h"
#include "hvn3/utility/BitFlags.h"

#include <utility>

namespace hvn3 {

	enum class CohenSutherlandOutCode {
		Inside = 0b0000,
		Left = 0b0001,
		Right = 0b0010,
		Bottom = 0b0100,
		Top = 0b1000
	};
	HVN3_ENABLE_BITFLAGS(CohenSutherlandOutCode);

	template <typename T>
	CohenSutherlandOutCode GetCohenSutherlandOutCode(const Rectangle<T>& rectangle, const Point2d<T>& point) {

		CohenSutherlandOutCode out_code = CohenSutherlandOutCode::Inside;

		if (point.X() < rectangle.Left())
			out_code |= CohenSutherlandOutCode::Left;
		else if (point.X() > rectangle.Right())
			out_code |= CohenSutherlandOutCode::Right;

		if (point.Y() < rectangle.Top())
			out_code |= CohenSutherlandOutCode::Top;
		else if (point.Y() > rectangle.Bottom())
			out_code |= CohenSutherlandOutCode::Bottom;

		return out_code;

	}
	template <typename T>
	std::pair<CohenSutherlandOutCode, CohenSutherlandOutCode> GetCohenSutherlandOutCodes(const Rectangle<T>& rectangle, const Line<T>& line) {

		auto result = std::make_pair(GetCohenSutherlandOutCode(rectangle, line.First()), GetCohenSutherlandOutCode(rectangle, line.Second()));

		return result;

	}

}