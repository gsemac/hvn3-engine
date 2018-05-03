#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/graphics/Display.h"

namespace hvn3 {

	class DisplayEventArgs : public hvn3::System::EventArgs {
	public:
		DisplayEventArgs(Display* display) {
			_display = display;
		}
		const Display& Display() const {
			return *_display;
		}
	private:
		hvn3::Display* _display;
	};

	class DisplaySizeChangedEventArgs : public DisplayEventArgs {
	public:
		using DisplayEventArgs::DisplayEventArgs;
		const SizeI& NewSize() const {
			return this->Display().Size();
		}
	};

}