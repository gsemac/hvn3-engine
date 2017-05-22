#include "DrawEventArgs.h"

namespace hvn3 {

	DrawEventArgs::DrawEventArgs(Drawing::Graphics& graphics) : __graphics(graphics) {}
	Drawing::Graphics& DrawEventArgs::Graphics() {

		return __graphics;

	}

}