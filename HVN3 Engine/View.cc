#include <algorithm>
#include "View.h"
#include "Object.h"
#include "Helper.h"
#include "Mouse.h"
#include "Room.h"
#include "Transform.h"

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

	return Rectangle(__view_pos.X(), __view_pos.Y(), __view_size.Width(), __view_size.Height());

}
Rectangle View::Port() const {

	return Rectangle(__port_pos.X(), __port_pos.Y(), __port_size.Width(), __port_size.Height());

}
const Point& View::Position() const {

	return __view_pos;

}
void View::SetPosition(float x, float y) {

	__view_pos.SetXY(x, y);

}
void View::SetPosition(const Point& position) {

	__view_pos = position;

}
float View::ViewX() const {

	return __view_pos.X();

}
float View::ViewY() const {

	return __view_pos.Y();

}
Scale View::Scale() const {

	return ::Scale(Port().Width() / Region().Width(), Port().Height() / Region().Height());

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

bool View::TracksMouse() const {

	return __tracks_mouse;

}
void View::TracksMouse(bool tracks) {

	__tracks_mouse = tracks;

}
