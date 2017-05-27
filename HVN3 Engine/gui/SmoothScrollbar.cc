#include "SmoothScrollbar.h"
#include "IScrollable.h"

namespace hvn3 {
	namespace Gui {

		SmoothScrollbar::SmoothScrollbar(IScrollable* control, Point position, Size size) :
			SmoothScrollbar(control, position, size, Orientation::Vertical) {
		}
		SmoothScrollbar::SmoothScrollbar(IScrollable* control, Point position, Size size, Orientation orientation) :
			Scrollbar(control, position, size, orientation) {

			_ease_position = ScrollPercentage();

		}

		void SmoothScrollbar::OnResize(ResizeEventArgs& e) {

			Scrollbar::OnResize(e);

			// Don't ease into the new scrollbar positions on a resize.
			_ease_position = Scrollbar::_Position();
			_ScrollTargetToPosition();

		}
		void SmoothScrollbar::Update(UpdateEventArgs& e) {

			Scrollbar::Update(e);

			// Ease towards the target position.
			float diff = Diff(_ease_position, Scrollbar::_Position());
			if (diff > 0.0f) {

				if (diff <= 0.1f)
					_ease_position = Scrollbar::_Position();
				else
					_ease_position = EaseTowards(_ease_position, Scrollbar::_Position(), diff / 10.0f);

				_ScrollTargetToPosition();

			}

		}

		// Protected methods

		void SmoothScrollbar::_ScrollTargetToPosition() {

			_Target()->OnScroll(_GetScrollEventArgs(_PositionToPercentage(_ease_position)));

		}

	}
}