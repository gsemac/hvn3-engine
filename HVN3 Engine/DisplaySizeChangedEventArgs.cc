#include "DisplaySizeChangedEventArgs.h"

DisplaySizeChangedEventArgs::DisplaySizeChangedEventArgs(const Size& old_size, const Size& new_size, ::Display* display) :
	_previous_size(old_size),
	_new_size(new_size),
	_display(display) {
}
const Size& DisplaySizeChangedEventArgs::OldSize() const {

	return _previous_size;

}
const Size& DisplaySizeChangedEventArgs::NewSize() const {

	return _new_size;

}
const Display* DisplaySizeChangedEventArgs::Display() const {

	return _display;

}