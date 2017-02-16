#pragma once
#include "Transform.h"
#include "Rectangle.h"

namespace Drawing {

	class GraphicsState {
		friend class Graphics;

	private:
		GraphicsState(const Graphics& graphics);

		Transform __transform;
		Rectangle __clip;

	};

}