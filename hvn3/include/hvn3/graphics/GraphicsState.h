#pragma once
#include "hvn3/graphics/Transform.h"
#include "hvn3/math/Rectangle.h"

namespace hvn3 {
	namespace Graphics {

		class Graphics;

		class GraphicsState {
			friend class Graphics;

		private:
			GraphicsState(const Graphics& graphics);

			Transform __transform;
			RectangleF __clip;

		};

	}
}