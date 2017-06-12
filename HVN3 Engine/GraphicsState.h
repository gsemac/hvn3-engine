#pragma once
#include "Transform.h"
#include "Rectangle.h"

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