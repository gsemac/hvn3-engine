#include "gui/Scrollbar.h"
#include "gui/IScrollable.h"
#include "io/Mouse.h"
#include "utility/Utility.h"
#include "graphics/Color.h"
#define MINIMUM_SLIDER_SIZE 10.0f

namespace hvn3 {

	namespace Gui {

		Scrollbar::Scrollbar(IScrollable* control, PointF position, SizeF size) :
			Scrollbar(control, position, size, Orientation::Vertical) {
		}
		Scrollbar::Scrollbar(IScrollable* control, PointF position, SizeF size, Orientation orientation) :
			Control(position, size),
			_orientation(orientation),
			_position(0),
			_dragging(false) {

			SetTarget(control);
			_recalculateSliderSize();

		}

		void Scrollbar::SetTarget(IScrollable* target) {

			// Set the new target.
			_target = target;

		}
		void Scrollbar::SetScrollPercentage(float percent) {

			// Make sure that percentage value is valid.
			percent = Math::Clamp(percent, 0.0f, 1.0f);

			// Adjust the position of the slider.
			if (_orientation == Orientation::Vertical)
				_setPosition((Height() - _slider_height) * percent);
			else
				_setPosition((Width() - _slider_height) * percent);

		}
		float Scrollbar::ScrollPercentage() const {

			return _positionToPercentage(_getPosition());

		}
		void Scrollbar::ScrollToTop() {

			SetScrollPercentage(0);

		}
		void Scrollbar::ScrollToBottom() {

			SetScrollPercentage(1.0f);

		}

		void Scrollbar::OnResize(ResizeEventArgs& e) {

			// Relculate the size of the slider to compensate for the new size.
			_recalculateSliderSize();

			// Invalidate the control so slider is redrawn.
			Invalidate();

		}
		void Scrollbar::OnMouseEnter() {}
		void Scrollbar::OnMouseLeave() {

			// Invalidate the control to update the highlight color.
			Invalidate();

		}
		void Scrollbar::OnMouseDown(MouseEventArgs& e) {

			if (_mouseOnSlider() && Mouse::ButtonPressed(MouseButton::Left)) {

				_mouse_clicked_pos = (_orientation == Orientation::Vertical ? Mouse::Y : Mouse::X);
				_starting_position = _position;
				_dragging = true;

			}

		}
		void Scrollbar::OnMouseUp(MouseEventArgs& e) {


		}
		void Scrollbar::OnMouseMove(MouseMoveEventArgs& e) {

			// Invalidate the control to update the highlight color.
			Invalidate();

		}
		void Scrollbar::OnUpdate(UpdateEventArgs& e) {

			// Stop dragging when the mouse button is released.
			if (!Mouse::ButtonDown(MouseButton::Left))
				_dragging = false;

			if (_dragging) {

				// Calculate the new position of the slider.
				float new_position;
				if (_orientation == Orientation::Vertical)
					new_position = Math::Min(Height() - _slider_height, Math::Max(0.0f, _starting_position - (_mouse_clicked_pos - Mouse::Y)));
				else
					new_position = Math::Min(Width() - _slider_height, Math::Max(0.0f, _starting_position - (_mouse_clicked_pos - Mouse::X)));

				if (new_position != _getPosition()) {
				
					// Update the position of the slider (if it has changed).
					_setPosition(new_position);

				}

			}
			else if (_target && (_target->HasFocus() || HasFocus())) {

				if (_orientation == Orientation::Vertical && (Mouse::ScrolledDown() || Mouse::ScrolledUp()))
					SetScrollPercentage(_pixelsToPercentage(_percentageToPixels(ScrollPercentage()) + (Mouse::ScrolledDown() ? -10 : 10)));

				else if (_orientation == Orientation::Horizontal && (Mouse::ScrolledLeft() || Mouse::ScrolledRight()))
					SetScrollPercentage(_pixelsToPercentage(_percentageToPixels(ScrollPercentage()) + (Mouse::ScrolledLeft() ? -10 : 10)));

			}

		}
		void Scrollbar::OnPaint(PaintEventArgs& e) {

			e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor().Lighter());

			if (_orientation == Orientation::Vertical)
				e.Graphics().DrawFilledRectangle(0, _position, Width(), _slider_height, _mouseOnSlider() ? Color::LtGrey : Color::Grey);
			else
				e.Graphics().DrawFilledRectangle(_position, 0, _slider_height, Height(), _mouseOnSlider() ? Color::LtGrey : Color::Grey);

		}

		// Protected methods

		void Scrollbar::_setPosition(float position) {

			// Update the position value.
			_position = position;

			// Update the position of the control.
			if (_target)
				_scrollTargetToPosition();

			// Invalidate the scrollbar to draw the new slider position.
			Invalidate();

		}
		float Scrollbar::_getPosition() const {

			return _position;

		}
		float Scrollbar::_positionToPercentage(float position) const {

			if (_orientation == Orientation::Vertical)
				if (Height() == _slider_height)
					return 0.0f;
				else
					return position / (Height() - _slider_height);
			else
				if (Width() == _slider_height)
					return 0.0f;
				else
					return position / (Width() - _slider_height);

		}
		ScrollEventArgs Scrollbar::_getScrollEventArgs(float scroll_percentage) const {

			if (_orientation == Orientation::Vertical)
				return ScrollEventArgs(scroll_percentage, (_target->ScrollableRegion().Height() - _target->VisibleRegion().Height()) * scroll_percentage, _orientation);
			else
				return ScrollEventArgs(scroll_percentage, (_target->ScrollableRegion().Width() - _target->VisibleRegion().Width()) * scroll_percentage, _orientation);

		}
		IScrollable* Scrollbar::_getTarget() {

			return _target;

		}
		Orientation Scrollbar::_getOrientation() const {

			return _orientation;
		
		}
		void Scrollbar::_scrollTargetToPosition() {

			_getTarget()->OnScroll(_getScrollEventArgs(ScrollPercentage()));

		}

		// Private methods

		bool Scrollbar::_mouseOnSlider() {

			if (!IsActiveControl())
				return false;

			PointF fp = FixedPosition();

			if (_orientation == Orientation::Vertical)
				return Mouse::InRegion(fp.X(), fp.Y() + _position, fp.X() + Width(), fp.Y() + _position + _slider_height);
			else
				return Mouse::InRegion(fp.X() + _position, fp.Y(), fp.X() + _position + _slider_height, fp.Y() + Height());

		}
		void Scrollbar::_recalculateSliderSize() {

			// Adjust the height of the slider.
			if (_orientation == Orientation::Vertical)
				if (_target == nullptr || _target->ScrollableRegion().Height() < Height())
					_slider_height = Height();
				else
					_slider_height = Math::Max(0.0f, (_target->VisibleRegion().Height() / _target->ScrollableRegion().Height()) * Height());
			else
				if (_target == nullptr || _target->ScrollableRegion().Width() < Width())
					_slider_height = Width();
				else
					_slider_height = Math::Max(0.0f, (_target->VisibleRegion().Width() / _target->ScrollableRegion().Width()) * Width());

			// Set a minimum size for the slider so it doesn't become unclickable.
			_slider_height = Math::Max(_slider_height, MINIMUM_SLIDER_SIZE);

			// Adjust the scroll position to compensate for the new height of the slider.
			// We want to maintain the same scroll position unless the visible region becomes large enough that we can no longer scroll that far.
			SetScrollPercentage(ScrollPercentage());

		}
		int Scrollbar::_percentageToPixels(float percentage) {

			return static_cast<int>((Height() - _slider_height) * percentage);

		}
		float Scrollbar::_pixelsToPercentage(int pixels) {

			return (float)pixels / (Height() - _slider_height);

		}

	}

}