#include "gui/Scrollbar.h"
#include "gui/IScrollable.h"
#include "io/Mouse.h"
#include "Utility.h"
#include "Color.h"

namespace hvn3 {

	namespace Gui {

		Scrollbar::Scrollbar(IScrollable* control, Point position, Size size) :
			Scrollbar(control, position, size, Orientation::Vertical) {
		}
		Scrollbar::Scrollbar(IScrollable* control, Point position, Size size, Orientation orientation) :
			Control(position, size),
			_orientation(orientation),
			_position(0),
			_dragging(false) {

			SetTarget(control);
			RecalculateSliderSize();

		}

		void Scrollbar::SetTarget(IScrollable* target) {

			// Set the new target.
			_target = target;

		}
		void Scrollbar::SetScrollPercentage(float percent) {

			// Make sure that percentage value is valid.
			percent = Clamp(percent, 0.0f, 1.0f);

			// Adjust the position of the slider.
			if (_orientation == Orientation::Vertical)
				_position = (Height() - _slider_height) * percent;
			else
				_position = (Width() - _slider_height) * percent;

			// Adjust the scroll value of the associated object.
			if (_target != nullptr)
				ScrollTargetToPosition();

		}
		float Scrollbar::ScrollPercentage() const {

			if (_orientation == Orientation::Vertical)
				if (Height() == _slider_height)
					return 0.0f;
				else
					return _position / (Height() - _slider_height);
			else
				if (Width() == _slider_height)
					return 0.0f;
				else
					return _position / (Width() - _slider_height);

		}
		void Scrollbar::ScrollToTop() {

			SetScrollPercentage(0);

		}
		void Scrollbar::ScrollToBottom() {

			SetScrollPercentage(1.0f);

		}

		void Scrollbar::OnResize(ResizeEventArgs& e) {

			// Relculate the size of the slider to compensate for the new size.
			RecalculateSliderSize();

			// Invalidate the control so slider is redrawn.
			Invalidate();

		}
		void Scrollbar::OnMouseEnter() {}
		void Scrollbar::OnMouseLeave() {

			// Invalidate the control to update the highlight color.
			Invalidate();

		}
		void Scrollbar::OnMouseDown() {

			if (MouseOnSlider() && Mouse::ButtonPressed(MB_LEFT)) {

				_mouse_clicked_pos = (_orientation == Orientation::Vertical ? Mouse::Y : Mouse::X);
				_starting_position = _position;
				_dragging = true;

			}

		}
		void Scrollbar::OnMouseUp() {


		}
		void Scrollbar::OnMouseMove() {

			// Invalidate the control to update the highlight color.
			Invalidate();

		}
		void Scrollbar::Update(UpdateEventArgs& e) {

			// Stop dragging when the mouse button is released.
			if (!Mouse::ButtonDown(MB_LEFT))
				_dragging = false;

			if (_dragging) {

				// Calculate the new position of the slider.
				float new_position;
				if (_orientation == Orientation::Vertical)
					new_position = Min(Height() - _slider_height, Max(0.0f, _starting_position - (_mouse_clicked_pos - Mouse::Y)));
				else
					new_position = Min(Width() - _slider_height, Max(0.0f, _starting_position - (_mouse_clicked_pos - Mouse::X)));

				if (new_position != _position) {

					// Update the position of the slider (if it has changed).
					_position = new_position;

					// If there is a target control, scroll it.
					if (_target)
						ScrollTargetToPosition();

					// Invalidate the scrollbar to draw the new slider position.
					Invalidate();

				}

			}
			else if (_target && (_target->HasFocus() || HasFocus())) {

				if (_orientation == Orientation::Vertical && (Mouse::ScrolledDown() || Mouse::ScrolledUp()))
					SetScrollPercentage(PixelsToPercentage(PercentageToPixels(ScrollPercentage()) + (Mouse::ScrolledDown() ? -10 : 10)));

				else if (_orientation == Orientation::Horizontal && (Mouse::ScrolledLeft() || Mouse::ScrolledRight()))
					SetScrollPercentage(PixelsToPercentage(PercentageToPixels(ScrollPercentage()) + (Mouse::ScrolledLeft() ? -10 : 10)));

			}

		}
		void Scrollbar::OnPaint(PaintEventArgs& e) {

			e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor().Lighter());

			if (_orientation == Orientation::Vertical)
				e.Graphics().DrawFilledRectangle(0, _position, Width(), _slider_height, MouseOnSlider() ? Color::LtGrey : Color::Grey);
			else
				e.Graphics().DrawFilledRectangle(_position, 0, _slider_height, Height(), MouseOnSlider() ? Color::LtGrey : Color::Grey);

		}

		// Private methods

		bool Gui::Scrollbar::MouseOnSlider() {

			if (!IsActiveControl())
				return false;

			Point fp = FixedPosition();

			if (_orientation == Orientation::Vertical)
				return Mouse::InRegion(fp.X(), fp.Y() + _position, fp.X() + Width(), fp.Y() + _position + _slider_height);
			else
				return Mouse::InRegion(fp.X() + _position, fp.Y(), fp.X() + _position + _slider_height, fp.Y() + Height());

		}
		void Gui::Scrollbar::RecalculateSliderSize() {

			// Adjust the height of the slider.
			if (_orientation == Orientation::Vertical)
				if (_target == nullptr || _target->ScrollableRegion().Height() < Height())
					_slider_height = Height();
				else
					_slider_height = Max(0.0f, (_target->VisibleRegion().Height() / _target->ScrollableRegion().Height()) * Height());
			else
				if (_target == nullptr || _target->ScrollableRegion().Width() < Width())
					_slider_height = Width();
				else
					_slider_height = Max(0.0f, (_target->VisibleRegion().Width() / _target->ScrollableRegion().Width()) * Width());

			// Set a minimum size for the slider so it doesn't become unclickable.
			_slider_height = Max(_slider_height, 15.0f);

			// Adjust the scroll position to compensate for the new height of the slider.
			// We want to maintain the same scroll position unless the visible region becomes large enough that we can no longer scroll that far.
			SetScrollPercentage(ScrollPercentage());

		}
		void Gui::Scrollbar::ScrollTargetToPosition() {

			if (_orientation == Orientation::Vertical)
				_target->OnScroll(ScrollEventArgs(ScrollPercentage(), (_target->ScrollableRegion().Height() - _target->VisibleRegion().Height()) * ScrollPercentage(), _orientation));
			else
				_target->OnScroll(ScrollEventArgs(ScrollPercentage(), (_target->ScrollableRegion().Width() - _target->VisibleRegion().Width()) * ScrollPercentage(), _orientation));

		}
		int Gui::Scrollbar::PercentageToPixels(float percentage) {

			return (Height() - _slider_height) * percentage;

		}
		float Gui::Scrollbar::PixelsToPercentage(int pixels) {

			return (float)pixels / (Height() - _slider_height);

		}

	}

}