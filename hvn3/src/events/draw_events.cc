#include "events/draw_events.h"

namespace hvn3::events {

	// Public members

	DrawEvent::DrawEvent(graphics::IGraphics& graphics) :
		graphics(graphics) {
	}

	graphics::IGraphics& DrawEvent::Graphics() {

		return graphics;

	}
	const graphics::IGraphics& DrawEvent::Graphics() const {

		return graphics;

	}

}