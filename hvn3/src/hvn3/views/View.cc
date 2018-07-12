#include <algorithm>
#include "hvn3/views/View.h"
#include "hvn3/objects/Object.h"
#include "hvn3/utility/Utility.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/rooms/RoomBase.h"

namespace hvn3 {

	View::View(float x, float y, float width, float height) :
		View(x, y, width, height, 0.0f, 0.0f, width, height) {
	}
	View::View(float x, float y, float width, float height, float port_x, float port_y, float port_width, float port_height) :
		View(PointF(x, y), SizeF(width, height), PointF(port_x, port_y), SizeF(port_width, port_height)) {
	}
	View::View(const PointF& position, const SizeF& size) :
		View(position, size, PointF(0.0f, 0.0f), size) {
	}
	View::View(const PointF& position, const SizeF& size, const PointF& port_position, const SizeF& port_size) :
		_size(size),
		_port_size(port_size) {

		_position = position;
		_port_position = port_position;

		_horizontal_border = 0.0f;
		_vertical_border = 0.0f;

		_vspeed = 0.0f;
		_hspeed = 0.0f;
		_angle = 0.0f;
		_enabled = true;
		_tracks_mouse = true;

	}
	View::View(const PointF& position, const SizeF& size, const PointF& port_position, const SizeF& port_size, IObject* following, const SizeF& border) :
		View(position, size, port_position, port_size) {

		_following = following;
		_horizontal_border = (std::min)(border.Width(), size.Width() / 2.0f);
		_vertical_border = (std::min)(border.Height(), size.Height() / 2.0f);

	}

	void View::SetFollowing(IObject* following) {
		_following = following;
	}
	void View::SetFollowing(IObject* following, float border_width, float border_height) {
		_following = following;
		_horizontal_border = border_width;
		_vertical_border = border_height;
	}
	void View::SetFollowing(IObject* following, const SizeF& border) {
		SetFollowing(following, border.Width(), border.Height());
	}
	const IObject* View::GetFollowing() const {
		return _following;
	}
	IObject* View::GetFollowing() {
		return _following;
	}

	RectangleF View::Region() const {

		return RectangleF(_position.X(), _position.Y(), _size.Width(), _size.Height());

	}
	RectangleF View::Port() const {

		return RectangleF(_port_position.X(), _port_position.Y(), _port_size.Width(), _port_size.Height());

	}
	SizeF View::Size() const {
		return Region().Size();
	}
	const PointF& View::Position() const {

		return _position;

	}
	void View::SetPosition(float x, float y) {

		_position.SetX(x);
		_position.SetY(y);

	}
	void View::SetPosition(const PointF& position) {

		_position = position;

	}
	float View::X() const {

		return _position.X();

	}
	float View::Y() const {

		return _position.Y();

	}
	Scale View::Scale() const {

		return hvn3::Scale(Port().Width() / Region().Width(), Port().Height() / Region().Height());

	}

	float View::HorizontalBorder() const {

		return _horizontal_border;

	}
	float View::VerticalBorder() const {

		return _vertical_border;

	}

	float View::Angle() const {

		return _angle;

	}
	void View::SetAngle(float angle) {

		_angle = std::fmod(angle, 360.0f);

	}

	void View::SetEnabled(bool value) {
		_enabled = value;
	}
	bool View::Enabled() const {
		return _enabled;
	}

	void View::SetMouseTrackingEnabled(bool value) {
		_tracks_mouse = value;
	}
	bool View::MouseTrackingEnabled() const {
		return _tracks_mouse;
	}

	Graphics::Transform View::GetTransform() const {

		PointF offset(X(), Y());
		Graphics::Transform transform;
		transform.Translate(-offset.X() + Port().X(), -offset.Y() + Port().Y());
		transform.Rotate(Region().Midpoint() - offset, Angle());
		transform.Scale(Scale());

		return transform;

	}

}