#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class Display;

	class DisplaySizeChangedEventArgs : public System::EventArgs {

	public:
		DisplaySizeChangedEventArgs(const Size<int>& old_size, const Size<int>& new_size, Display* display);
		const Size<int>& OldSize() const;
		const Size<int>& NewSize() const;
		const Display* Display() const;

	private:
		Size<int> _previous_size;
		Size<int> _new_size;
		hvn3::Display* _display;

	};

}