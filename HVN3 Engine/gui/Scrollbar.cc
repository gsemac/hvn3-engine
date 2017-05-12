#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <iostream>
#include "Scrollbar.h"
#include "IScrollable.h"
#include "io/Mouse.h"
#include "Color.h"

namespace Gui {

	Scrollbar::Scrollbar(IScrollable* control, Point position, Size size) :
		Scrollbar(control, position, size, Orientation::Vertical) {
	}
	Scrollbar::Scrollbar(IScrollable* control, Point position, Size size, Orientation orientation) :
		Control(position, size),
		_target(control),
		_orientation(orientation),
		_position(0),
		_dragging(false) {

		RecalculateSliderSize();

	}

	void Scrollbar::SetTarget(IScrollable* target) {

		// Set the new target.
		_target = target;

	}
	void Scrollbar::SetScrollPosition(float percent) {

		// Make sure that percentage value is valid.
		percent = Clamp(percent, 0.0f, 1.0f);

		// Adjust the position of the slider.
		_position = (Height() - _slider_height) * percent;

		// Adjust the scroll value of the associated object.
		if (_target != nullptr)
			_target->Scroll(ScrollPosition());

	}
	float Scrollbar::ScrollPosition() const {

		return _position / (Height() - _slider_height);

	}
	void Scrollbar::ScrollToTop() {

		SetScrollPosition(0);

	}
	void Scrollbar::ScrollToBottom() {

		SetScrollPosition(1.0f);

	}

	void Scrollbar::OnResize() {

		// Relculate the size of the slider to compensate for the new size.
		RecalculateSliderSize();

	}
	void Scrollbar::Update(UpdateEventArgs& e) {
		Invalidate();
		if (MouseOnSlider() && Mouse::ButtonPressed(MB_LEFT)) {

			_mouse_clicked_pos = Mouse::Y;
			_starting_position = _position;
			_dragging = true;

		}
		else if (Mouse::ButtonReleased(MB_LEFT)) {

			_dragging = false;
		}

		if (_dragging) {
			_position = Min(Height() - _slider_height, std::max((float)0, _starting_position - (_mouse_clicked_pos - Mouse::Y)));
			if (_target) _target->Scroll(ScrollPosition());
		}
		else if ((Mouse::ScrolledDown() || Mouse::ScrolledUp()) && _target && _target->HasFocus()) {
			SetScrollPosition(ScrollPosition() + ((Mouse::ScrolledDown()) ? -0.05f : 0.05f));
		}

		// Update the slider height according to the maximum scroll value.
		RecalculateSliderSize();

	}
	void Scrollbar::OnPaint(PaintEventArgs& e) {
		
		e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());
		e.Graphics().DrawFilledRectangle(0, _position, Width(), _slider_height, MouseOnSlider() ? Color::White : Color::LtGrey);

	}

	// Private methods
	bool Gui::Scrollbar::MouseOnSlider() {

		Point fp = FixedPosition();

		return (Mouse::X > fp.X() && Mouse::X < fp.X() + Width() && Mouse::Y > fp.Y() + _position && Mouse::Y < fp.Y() + _position + _slider_height);

	}
	void Gui::Scrollbar::RecalculateSliderSize() {

		//// Default to 0 for invalid/missing parameters.
		//if (!_target || _target->ScrollMax < Height()) {
		//	_slider_height = 0;
		//	return;
		//}

		// Adjust the height of the slider.
		_slider_height = (std::max)((float)0, (Height() / 2000) * Height());

	}

}