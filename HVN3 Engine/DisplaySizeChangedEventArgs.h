#pragma once
#include "EventArgs.h"
#include "Size.h"

namespace hvn3 {

	class Display;

	class DisplaySizeChangedEventArgs : public EventArgs {

	public:
		DisplaySizeChangedEventArgs(const Size& old_size, const Size& new_size, Display* display);
		const Size& OldSize() const;
		const Size& NewSize() const;
		const Display* Display() const;

	private:
		Size _previous_size;
		Size _new_size;
		hvn3::Display* _display;

	};

}