#include "hvn3/events/DrawEventArgs.h"

namespace hvn3 {

	DrawEventArgs::DrawEventArgs(Graphics::Graphics& canvas) :
		_graphics(canvas) {
	}

	Graphics::Graphics& DrawEventArgs::Graphics() {
		return _graphics;
	}

}