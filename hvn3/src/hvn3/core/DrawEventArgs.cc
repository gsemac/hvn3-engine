#include "hvn3/core/DrawEventArgs.h"

namespace hvn3 {

	DrawEventArgs::DrawEventArgs(Graphics::Graphics& graphics) : __graphics(graphics) {}
	Graphics::Graphics& DrawEventArgs::Graphics() {

		return __graphics;

	}

}