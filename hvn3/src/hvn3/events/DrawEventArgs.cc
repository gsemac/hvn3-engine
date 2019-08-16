#include "hvn3/events/DrawEventArgs.h"

namespace hvn3 {

	DrawEventArgs::DrawEventArgs(const ApplicationContext& context, Graphics::Graphics& canvas) :
		EventArgs(context),
		_graphics(canvas) {
	}

	Graphics::Graphics& DrawEventArgs::Graphics() {
		return _graphics;
	}

}