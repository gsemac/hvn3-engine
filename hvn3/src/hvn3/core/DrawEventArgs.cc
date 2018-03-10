#include "hvn3/core/DrawEventArgs.h"

namespace hvn3 {

	DrawEventArgs::DrawEventArgs(Graphics::Graphics& graphics) :
		_graphics(graphics) {
	}

	Graphics::Graphics& DrawEventArgs::Graphics() {

		return _graphics;

	}

}