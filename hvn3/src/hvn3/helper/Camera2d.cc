#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/helper/Camera2d.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/views/ViewManager.h"
#include <cassert>

namespace hvn3 {

	Camera2d::Camera2d() :
		ObjectBase(SystemObjects::CAMERA_2D) {

		_target = nullptr;
		_target_id = NoOne;
		_pan_mode = PanMode::EaseOut;
		_tilt_enabled = false;

	}
	Camera2d::Camera2d(float x, float y) :
		Camera2d() {
		ObjectBase::SetPosition(PointF(x, y));
	}
	Camera2d::Camera2d(const PointF& position) :
		Camera2d(position.x, position.y) {
	}
	Camera2d::Camera2d(IObject* target) :
		Camera2d(target->Position()) {

		_target = target;
		_target_id = target->Id();

	}
	Camera2d::Camera2d(ObjectId target_id) :
		Camera2d() {

		_target = nullptr;
		_target_id = target_id;

	}
	void Camera2d::OnUpdate(UpdateEventArgs& e) {

		PointF to = _getTargetPosition() + _offset;
		float dist = hvn3::Math::Geometry::PointDistance(to, Position());
		float max_speed = 0.0f;

		switch (_pan_mode) {
		case PanMode::Immediate:
			max_speed = dist;
			break;
		case PanMode::EaseOut:
			max_speed = dist / 10.0f;
			break;
		case PanMode::Linear:
			max_speed = Math::Min(dist, 10.0f); // #todo Make this customizable
			break;
		}

		ObjectBase::SetPosition(hvn3::Math::Geometry::PointInDirection(Position(), to, max_speed));

		// Rotate the view according to the horizontal speed of the camera.

		if (_tilt_enabled) {

			View* view = _getView();

			if (view != nullptr) {
				view->SetAngle(view->Angle() + Math::Signum(to.x - X()) * (dist / 180.0f));
				view->SetAngle(view->Angle() * 0.95f);
			}

		}

	}
	void Camera2d::SetPosition(const PointF& position) {

		ObjectBase::SetPosition(position);

		// Clear the target object when the position is set manually.
		_target = nullptr;
		_target_id = NoOne;

	}
	void Camera2d::SetFollowing(IObject* target) {

		_target = target;

		if (_target != nullptr)
			_target_id = _target->Id();
		else
			_target_id = NoOne;

	}
	void Camera2d::SetFollowing(ObjectId target_id) {

		if (_target_id == target_id)
			return;

		_target = nullptr;
		_target_id = target_id;

	}
	void Camera2d::SetOffset(float offset_x, float offset_y) {
		SetOffset(PointF(offset_x, offset_y));
	}
	void Camera2d::SetOffset(const PointF& offset) {
		_offset = offset;
	}
	void Camera2d::SetPanMode(PanMode value) {
		_pan_mode = value;
	}
	void Camera2d::SetTiltEnabled(bool value) {
		_tilt_enabled = value;
	}

	PointF Camera2d::_getTargetPosition() {

		if (_hasFollowingTarget()) {

			IObject* ptr = nullptr;

			// We need to make sure that the object still exists before trying to access it.

			if (_target_id != NoOne) {

				// If we have an ID to work with, we can find the object that way.
				while (ptr = Context().Objects().FindNext(_target_id), ptr != nullptr)
					if (ptr == _target)
						break;

			}
			else {

				// If we don't have an ID to work with, we can find the object by its address.
				Context().Objects().ForEach([&](const IObjectPtr& i) {
					if (ptr == i.get())
						ptr = i.get();
				});

			}

			if (ptr != nullptr)
				return ptr->Position();

		}

		return Position();

	}
	bool Camera2d::_hasFollowingTarget() {
		return !(_target_id == NoOne && _target == nullptr);
	}
	View* Camera2d::_getView() {

		for (size_t i = 0; i < Context().Views().Count(); ++i)
			if (Context().Views().At(i).GetFollowing() == this)
				return &Context().Views().At(i);

		return nullptr;

	}

}