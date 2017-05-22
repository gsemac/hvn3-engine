#include "GraphicsState.h"
#include "Graphics.h"

namespace hvn3 {

	namespace Drawing {

		GraphicsState::GraphicsState(const Graphics& graphics) :
			__transform(graphics.GetTransform()),
			__clip(graphics.Clip()) {
		}

	}

}