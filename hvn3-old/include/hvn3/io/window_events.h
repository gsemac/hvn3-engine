#pragma once

#include "hvn3/events/EventArgs.h"
#include "hvn3/io/window.h"
#include "hvn3/utility/Size.h"

namespace hvn3::io {

	class WindowEventArgs :
		public EventArgs {

	public:
		WindowEventArgs(const class Window& window);

		const class Window& Window() const;

	private:
		const class Window* window;

	};

	class WindowSizeChangedEventArgs :
		public WindowEventArgs {

	public:

		using WindowEventArgs::WindowEventArgs;

		SizeI NewSize() const;

	};

	typedef WindowEventArgs WindowClosedEventArgs;
	typedef WindowEventArgs WindowLostEventArgs;
	typedef WindowEventArgs WindowFoundEventArgs;

}