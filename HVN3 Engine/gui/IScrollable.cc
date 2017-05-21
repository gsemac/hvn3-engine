#include "gui/IScrollable.h"
#include "gui/Control.h"

namespace Gui {

	IScrollable::IScrollable(Control* control) :
		IScrollable(control, control->Height()) {
	}
	IScrollable::IScrollable(Control* control, float scroll_height) :
		IScrollable(control, Size(scroll_height, control->Width())) {
	}
	IScrollable::IScrollable(Control* control, const Size& scrollable_region) :
		_visible_region(control->Width(), control->Height()),
		_scrollable_region(scrollable_region) {

		_control = control;
		_auto_scroll_enabled = true;

	}

	void IScrollable::OnScroll(ScrollEventArgs& e) {

		if (e.Orientation() == Orientation::Vertical)
			_vpos = e.Position();
		else
			_hpos = e.Position();

		_control->Invalidate();

	}

	void IScrollable::SetVisibleRegion(const Size& region) {

		_visible_region = region;

	}
	const Size& IScrollable::VisibleRegion() const {

		return _visible_region;

	}
	void IScrollable::SetScrollableRegion(const Size& region) {

		_scrollable_region = region;

	}
	const Size& IScrollable::ScrollableRegion() const {

		return _scrollable_region;

	}

	bool IScrollable::AutoScrollEnabled() const {

		return _auto_scroll_enabled;

	}
	void IScrollable::SetAutoScrollEnabled(bool enabled) {

		_auto_scroll_enabled = enabled;

	}

	Point IScrollable::ScrollPosition() const {

		return Point(_hpos, _vpos);

	}
	bool IScrollable::HasFocus() {

		return _control->HasFocus();

	}

}