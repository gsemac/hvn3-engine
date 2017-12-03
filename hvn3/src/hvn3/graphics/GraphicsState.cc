#include "hvn3/graphics/GraphicsState.h"
#include "hvn3/graphics/Graphics.h"

namespace hvn3 {

	namespace Graphics {

		GraphicsState::GraphicsState(const Graphics& graphics) :
			__transform(graphics.GetTransform()),
			__clip(graphics.Clip()) {
		}

	}

}