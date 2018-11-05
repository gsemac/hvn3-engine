#pragma once

#include "hvn3/graphics/Color.h"

namespace hvn3 {
	namespace Graphics {

		class LinearGradientBrush {

		public:
			LinearGradientBrush(const Color& start_color, const Color& end_color);

			const Color& StartColor() const;
			const Color& EndColor() const;

		private:
			Color _start_color;
			Color _end_color;

		};

	}
}