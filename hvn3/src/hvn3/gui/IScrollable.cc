#include "hvn3/gui/IScrollable.h"
#include "hvn3/gui/Control.h"

namespace hvn3 {

	namespace Gui {

		IScrollable::IScrollable(Control* control) :
			IScrollable(control, control->Height()) {
		}
		IScrollable::IScrollable(Control* control, float scroll_height) :
			IScrollable(control, Size<float>(scroll_height, control->Width())) {
		}
		IScrollable::IScrollable(Control* control, const Size<float>& scrollable_region) :
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

		void IScrollable::SetVisibleRegion(const Size<float>& region) {

			_visible_region = region;

		}
		const Size<float>& IScrollable::VisibleRegion() const {

			return _visible_region;

		}
		void IScrollable::SetScrollableRegion(const Size<float>& region) {

			_scrollable_region = region;

		}
		const Size<float>& IScrollable::ScrollableRegion() const {

			return _scrollable_region;

		}

		bool IScrollable::AutoScrollEnabled() const {

			return _auto_scroll_enabled;

		}
		void IScrollable::SetAutoScrollEnabled(bool enabled) {

			_auto_scroll_enabled = enabled;

		}

		Point2d<float> IScrollable::ScrollPosition() const {

			return Point2d<float>(_hpos, _vpos);

		}
		bool IScrollable::HasFocus() {

			return _control->HasFocus();

		}

	}

}