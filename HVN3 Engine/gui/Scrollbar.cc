#include "gui/Scrollbar.h"
#include "gui/IScrollable.h"
#include "io/Mouse.h"
#include "Utility.h"
#include "Color.h"

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

	void Scrollbar::OnResize() {

		// Relculate the size of the slider to compensate for the new size.
		RecalculateSliderSize();

	}
	void Scrollbar::Update(UpdateEventArgs& e) {

		Invalidate();
		if (MouseOnSlider() && Mouse::ButtonPressed(MB_LEFT)) {

			_mouse_clicked_pos = (_orientation == Orientation::Vertical ? Mouse::Y : Mouse::X);
			_starting_position = _position;
			_dragging = true;

		}
		else if (Mouse::ButtonReleased(MB_LEFT)) {

			_dragging = false;
		}

		if (_dragging) {
			if (_orientation == Orientation::Vertical)
				_position = Min(Height() - _slider_height, Max(0.0f, _starting_position - (_mouse_clicked_pos - Mouse::Y)));
			else
				_position = Min(Width() - _slider_height, Max(0.0f, _starting_position - (_mouse_clicked_pos - Mouse::X)));
			if (_target)
				ScrollTargetToPosition();
		}
		else if ((Mouse::ScrolledDown() || Mouse::ScrolledUp()) && _target && _target->HasFocus()) {
			SetScrollPercentage(ScrollPercentage() + ((Mouse::ScrolledDown()) ? -0.05f : 0.05f));
		}

		// Update the slider height according to the maximum scroll value.
		RecalculateSliderSize();

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

	}
	void Gui::Scrollbar::ScrollTargetToPosition() {

		if (_orientation == Orientation::Vertical)
			_target->OnScroll(ScrollEventArgs(ScrollPercentage(), (_target->ScrollableRegion().Height() - _target->VisibleRegion().Height()) * ScrollPercentage(), _orientation));
		else
			_target->OnScroll(ScrollEventArgs(ScrollPercentage(), (_target->ScrollableRegion().Width() - _target->VisibleRegion().Width()) * ScrollPercentage(), _orientation));

	}

}