#include "hvn3/graphics/LinearGradientBrush.h"

namespace hvn3 {
	namespace Graphics {

		LinearGradientBrush::LinearGradientBrush(const Color& start_color, const Color& end_color) :
			_start_color(start_color),
			_end_color(end_color) {
		}
		const Color& LinearGradientBrush::StartColor() const {
			return _start_color;
		}
		const Color& LinearGradientBrush::EndColor() const {
			return _end_color;
		}

	}
}