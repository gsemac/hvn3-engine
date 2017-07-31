#include "SmoothScrollbar.h"
#include "IScrollable.h"

namespace hvn3 {
	namespace Gui {

		SmoothScrollbar::SmoothScrollbar(IScrollable* control, PointF position, SizeF size) :
			SmoothScrollbar(control, position, size, Orientation::Vertical) {
		}
		SmoothScrollbar::SmoothScrollbar(IScrollable* control, PointF position, SizeF size, Orientation orientation) :
			Scrollbar(control, position, size, orientation) {

			_ease_position = ScrollPercentage();

		}

		void SmoothScrollbar::OnResize(ResizeEventArgs& e) {

			Scrollbar::OnResize(e);

			// Don't ease into the new scrollbar positions on a resize.
			_ease_position = Scrollbar::_getPosition();
			_scrollTargetToPosition();

		}
		void SmoothScrollbar::OnUpdate(UpdateEventArgs& e) {

			Scrollbar::OnUpdate(e);

			// Ease towards the target position.
			float diff = Math::Diff(_ease_position, Scrollbar::_getPosition());
			if (diff > 0.0f) {

				if (diff <= 0.1f)
					_ease_position = Scrollbar::_getPosition();
				else
					_ease_position = EaseTowards(_ease_position, Scrollbar::_getPosition(), diff / 10.0f);

				_scrollTargetToPosition();

			}

		}

		// Protected methods

		void SmoothScrollbar::_scrollTargetToPosition() {

			_getTarget()->OnScroll(_getScrollEventArgs(_positionToPercentage(_ease_position)));

		}

	}
}