#include <algorithm>
#include "hvn3/views/View.h"
#include "hvn3/objects/Object.h"
#include "hvn3/utility/Utility.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/rooms/RoomBase.h"

namespace hvn3 {

	// Public methods

	View::View(float x, float y, float width, float height) :
		View(x, y, width, height, 0.0f, 0.0f, width, height) {
	}
	View::View(float x, float y, float width, float height, float portX, float portY, float portWidth, float portHeight) :
		View(PointF(x, y), SizeF(width, height), PointF(portX, portY), SizeF(portWidth, portHeight)) {
	}
	View::View(const PointF& position, const SizeF& size) :
		View(position, size, PointF(0.0f, 0.0f), size) {
	}
	View::View(const PointF& position, const SizeF& size, const PointF& portPosition, const SizeF& portSize) :
		View(RectangleF(position, size), RectangleF(portPosition, portSize)) {
	}
	View::View(const RectangleF& view, const RectangleF& port) {

		_view = view;
		_port = port;

		_horizontal_border = 0.0f;
		_vertical_border = 0.0f;

		_vspeed = 0.0f;
		_hspeed = 0.0f;
		_angle = 0.0f;
		_enabled = true;
		_tracks_mouse = true;

	}

	void View::SetTarget(const ecs::Entity& target) {
		_target = target;
	}
	void View::SetTarget(const ecs::Entity& target, float borderWidth, float borderHeight) {

		SetTarget(target);

		_horizontal_border = borderWidth;
		_vertical_border = borderHeight;

	}
	void View::SetTarget(const ecs::Entity& target, const SizeF& border) {
		SetTarget(target, border.width, border.height);
	}
	void View::ResetTarget() {
		_target.id = ecs::Entity::NULL_ENTITY_ID;
	}
	const ecs::Entity& View::Target() const {
		return _target;
	}

	const RectangleF& View::Region() const {
		return _view;
	}
	const RectangleF& View::Port() const {
		return _port;
	}
	SizeF View::Size() const {
		return Region().Size();
	}
	const PointF& View::Position() const {
		return _view.Position();
	}
	void View::SetPosition(float x, float y) {
		_view.SetPosition(x, y);
	}
	void View::SetPosition(const PointF& position) {
		_view.SetPosition(position);
	}
	void View::SetX(float x) {
		_view.SetX(x);
	}
	void View::SetY(float y) {
		_view.SetY(y);
	}
	float View::X() const {
		return _view.X();
	}
	float View::Y() const {
		return _view.Y();
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