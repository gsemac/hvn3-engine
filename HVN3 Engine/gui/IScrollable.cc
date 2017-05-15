#include "gui/IScrollable.h"
#include "gui/Control.h"

namespace Gui {

	IScrollable::IScrollable(Control* control) :
		IScrollable(control, control->Height()) {
	}
	IScrollable::IScrollable(Control* control, float scroll_height) :
		IScrollable(control, Rectangle(scroll_height, control->Width())) {
	}
	IScrollable::IScrollable(Control* control, Rectangle scrollable_region) :
		_visible_region(control->Width(), control->Height()),
		_scrollable_region(scrollable_region) {

		_control = control;

	}

	void IScrollable::OnScroll(ScrollEventArgs& e) {

		if (e.Orientation() == Orientation::Vertical)
			_vertical_scroll_pos = e.Percentage();
		else
			_horizontal_scroll_pos = e.Percentage();

		_control->Invalidate();

	}

	void IScrollable::SetVisibleRegion(const Rectangle& region) {

		_visible_region = region;

	}
	const Rectangle& IScrollable::VisibleRegion() const {

		return _visible_region;

	}
	void IScrollable::SetScrollableRegion(const Rectangle& region) {

		_scrollable_region = region;

	}
	const Rectangle& IScrollable::ScrollableRegion() const {

		return _scrollable_region;

	}

	bool IScrollable::HasFocus() {

		return _control->HasFocus();

	}

}