#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <iostream>
#include "GUIScrollbar.h"
#include "IScrollable.h"
#include "Mouse.h"
#include "Color.h"

// Public

GUI::Scrollbar::Scrollbar() : Scrollbar(nullptr, 0) {}
GUI::Scrollbar::Scrollbar(IScrollable* object, float height) {

	__obj = object;
	__height = height;
	__width = 20;
	__position = 0;
	__dragging = false;

	ResizeSlider();

}

void GUI::Scrollbar::SetHeight(float height) {

	// Calculate the percentage positioning of the slider.
	float per = PositionPercentage();

	// Update the height of the scrollbar/slider.
	__height = height;
	ResizeSlider();

	// Reposition the slider.
	if (__slider_height > 0) __position = (__height - __slider_height) * per;

}
void GUI::Scrollbar::SetTarget(IScrollable* target) {

	// Set the new target.
	__obj = target;

}
void GUI::Scrollbar::SetPosition(float percentage) {

	// Make sure that percentage value is valid.
	if (percentage > 1.0f) percentage = 1.0f;
	else if (percentage < 0) percentage = 0;

	// Adjust the position of the slider.
	__position = (__height - __slider_height) * percentage;

	// Adjust the scroll value of the associated object.
	if (__obj) __obj->Scroll(PositionPercentage());

}
void GUI::Scrollbar::ScrollToTop() {

	SetPosition(0);

}
void GUI::Scrollbar::ScrollToBottom() {

	SetPosition(1.0f);

}

float GUI::Scrollbar::Width() {

	if (!Shown()) return 0;
	return __width;

}
bool GUI::Scrollbar::Shown() {

	return true;
	//return (__obj->ScrollMax < __height);

}

void GUI::Scrollbar::Update() {

	if (Mouse::ButtonPressed(MB_LEFT) && MouseOn() && Shown()) {
		__mouse_clicked_pos = Mouse::Y;
		__starting_position = __position;
		__dragging = true;
	}
	else if (Mouse::ButtonReleased(MB_LEFT))
		__dragging = false;
	if (__dragging) {
		__position = std::min(__height - __slider_height, std::max((float)0, __starting_position - (__mouse_clicked_pos - Mouse::Y)));
		if (__obj) __obj->Scroll(PositionPercentage());
	}
	else if ((Mouse::ScrolledDown() || Mouse::ScrolledUp()) && __obj && __obj->HasFocus()) {
		SetPosition(PositionPercentage() + ((Mouse::ScrolledDown()) ? -0.05f : 0.05f));
	}



	// Update the slider height according to the maximum scroll value.
	ResizeSlider();

}
void GUI::Scrollbar::Draw() {

	if (!Shown()) return;
	al_draw_filled_rectangle(X(), Y(), X() + __width, X() + __height, Color::Grey.AlPtr());
	al_draw_filled_rectangle(X(), Y() + __position, X() + __width, Y() + __position + __slider_height, (MouseOn()) ? Color::White.AlPtr() : Color::LtGrey.AlPtr());

}

// Private

float GUI::Scrollbar::PositionPercentage() {

	return __position / (__height - __slider_height);

}
bool GUI::Scrollbar::MouseOn() {

	return (Mouse::X > X() && Mouse::X < X() + __width && Mouse::Y > Y() + __position && Mouse::Y < Y() + __position + __slider_height);

}
void GUI::Scrollbar::ResizeSlider() {

	// Default to 0 for invalid/missing parameters.
	if (!__obj || __obj->ScrollMax < __height) {
		__slider_height = 0;
		return;
	}

	// Adjust the height of the slider.
	__slider_height = (std::max)((float)0, (__height / __obj->ScrollMax) * __height);

}