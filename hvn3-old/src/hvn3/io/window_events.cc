#include "hvn3/io/window_events.h"

namespace hvn3::io {

	// Public members

	WindowEventArgs::WindowEventArgs(const class Window& window) {

		this->window = &window;

	}

	const class Window& WindowEventArgs::Window() const {

		return *window;

	}

	SizeI WindowSizeChangedEventArgs::NewSize() const {

		return Window().Size();

	}

}