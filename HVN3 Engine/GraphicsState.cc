#include "GraphicsState.h"
#include "Graphics.h"

namespace Drawing {

	GraphicsState::GraphicsState(const Graphics& graphics) :
	__transform(graphics.GetTransform()),
	__clip(graphics.Clip()) {}

}