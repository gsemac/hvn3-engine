#include <algorithm>
#include "View.h"
#include "Object.h"
#include "Helper.h"
#include "Mouse.h"
#include "Scene.h"

View::View(Point view_position, Size view_size, Point port_position, Size port_size, Object* follow_obj, float horizontal_border, float vertical_border) :__view_size(view_size), __port_size(port_size) {

	__view_pos = view_position;
	__port_pos = port_position;

	__follow_obj = follow_obj;
	__hbor = (std::min)(horizontal_border, view_size.Width() / 2.0f);
	__vbor = (std::min)(vertical_border, view_size.Height() / 2.0f);
	__vspd = 0.0f;
	__hspd = 0.0f;
	__angle = 0.0f;
	__enabled = false;
	__tracks_mouse = true;

}
View::View(Point view_position, Size view_size, Point port_position, Size port_size) : View(view_position, view_size, port_position, port_size, nullptr, 0, 0) {}

void View::SetFollowing(Object* obj) {

	__follow_obj = obj;

}
Object* View::GetFollowing() {

	return __follow_obj;

}

Rectangle View::Region() const {

	return Rectangle(__view_pos.X, __view_pos.Y, __view_size.Width(), __view_size.Height());

}
Rectangle View::Port() const {

	return Rectangle(__port_pos.X, __port_pos.Y, __port_size.Width(), __port_size.Height());

}
Point& View::ViewPosition() {

	return __view_pos;

}
float& View::ViewX() {

	return __view_pos.X;

}
float& View::ViewY() {

	return __view_pos.Y;

}
float View::ScaleX() const {

	return Port().Width() / Region().Width();

}
float View::ScaleY() const {

	return Port().Height() / Region().Height();

}

float View::HorizontalBorder() const {

	return __hbor;

}
float View::VerticalBorder() const {

	return __vbor;

}

float View::Angle() const {

	return __angle;

}
void View::SetAngle(float angle) {

	__angle = std::fmod(angle, 360.0f);

}

void View::Enable() {

	__enabled = true;

}
void View::Disable() {

	__enabled = false;

}
bool View::Enabled() const {

	return __enabled;

}

bool View::HasMouse() const {

	if (!__tracks_mouse) 
		return false;

	return PointIn(Mouse::DisplayPosition(), Port());

}
Point View::MousePosition() const {

	// Get the mouse position relative to the display.
	Point pos = Mouse::DisplayPosition();

	// Transform the mouse position against the view transformations.
	Graphics::Transform t;
	t.Rotate(Port().Midpoint(), -Angle());
	t.Translate(__view_pos.X * ScaleX(), __view_pos.Y * ScaleY());
	t.Scale(1.0f / ScaleX(), 1.0f / ScaleY());
	t.TransformPoint(pos);

	// Return the result.
	return pos;

}
bool View::TracksMouse() const {

	return __tracks_mouse;

}
void View::TracksMouse(bool tracks) {

	__tracks_mouse = tracks;

}