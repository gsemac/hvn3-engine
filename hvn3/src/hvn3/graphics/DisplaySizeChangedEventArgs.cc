#include "hvn3/graphics/DisplaySizeChangedEventArgs.h"

namespace hvn3 {

	DisplaySizeChangedEventArgs::DisplaySizeChangedEventArgs(const Size<int>& old_size, const Size<int>& new_size, hvn3::Display* display) :
		_previous_size(old_size),
		_new_size(new_size),
		_display(display) {
	}
	const Size<int>& DisplaySizeChangedEventArgs::OldSize() const {

		return _previous_size;

	}
	const Size<int>& DisplaySizeChangedEventArgs::NewSize() const {

		return _new_size;

	}
	const Display* DisplaySizeChangedEventArgs::Display() const {

		return _display;

	}

}