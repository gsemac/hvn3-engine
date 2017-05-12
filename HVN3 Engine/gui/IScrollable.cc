#include "gui/IScrollable.h"
#include "gui/Control.h"

namespace Gui {

	IScrollable::IScrollable(Control* control) :
		IScrollable(control, 0) {
	}
	IScrollable::IScrollable(Control* control, float vertical_max) :
		IScrollable(control, vertical_max, 0) {
	}
	IScrollable::IScrollable(Control* control, float horizontal_max, float vertical_max) {

		_control = control;
		_horizontal_max = horizontal_max;
		_vertical_max = vertical_max;

	}

	bool IScrollable::HasFocus() {

		return _control->HasFocus();

	}

}