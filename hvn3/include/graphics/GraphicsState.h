#pragma once
#include "graphics/Transform.h"
#include "math/Rectangle.h"

namespace hvn3 {

	namespace Drawing {

		class GraphicsState {
			friend class Graphics;

		private:
			GraphicsState(const Graphics& graphics);

			Transform __transform;
			RectangleF __clip;

		};

	}

}