//#include "hvn3/core/UpdateEventArgs.h"
//#include "hvn3/helper/Camera2d.h"
//#include "hvn3/math/GeometryUtils.h"
//#include "hvn3/objects/IObjectManager.h"
//#include "hvn3/utility/PerlinNoise.h"
//#include "hvn3/utility/Random.h"
//#include "hvn3/views/ViewManager.h"
//
//#include <cassert>
//
//namespace hvn3 {
//
//	Camera2d::Camera2d() :
//		ObjectBase(SystemObjects::CAMERA_2D) {
//
//		_target = nullptr;
//		_target_id = NoOne;
//		_pan_mode = PanMode::EaseOut;
//		_tilt_enabled = false;
//
//		_shake_seed = 0.0f;
//		_shake_max_angle = 0.0f;
//		_shake_max_offset = 0.0f;
//		_shake_duration = 0.0f;
//		_shake_duration_max = 0.0f;
//
//	}
//	Camera2d::Camera2d(float x, float y) :
//		Camera2d() {
//		ObjectBase::SetPosition(PointF(x, y));
//	}
//	Camera2d::Camera2d(const PointF& position) :
//		Camera2d(position.x, position.y) {
//	}
//	Camera2d::Camera2d(IObject* target) :
//		Camera2d(target->Position()) {
//
//		_target = target;
//		_target_id = target->Id();
//
//	}
//	Camera2d::Camera2d(ObjectId target_id) :
//		Camera2d() {
//
//		_target = nullptr;
//		_target_id = target_id;
//
//	}
//	void Camera2d::OnCreate(CreateEventArgs& e) {
//
//		SnapToTarget();
//
//	}
//	void Camera2d::OnUpdate(UpdateEventArgs& e) {
//
//		// Ease the camera towards the target object.
//
//		PointF to = _getTargetPosition() + _offset;
//		float dist = hvn3::Math::Geometry::PointDistance(to, Position());
//		float max_speed = 0.0f;
//
//		switch (_pan_mode) {
//		case PanMode::Immediate:
//			max_speed = dist;
//			break;
//		case PanMode::EaseOut:
//			max_speed = dist / 10.0f;
//			break;
//		case PanMode::Linear:
//			max_speed = Math::Min(dist, 10.0f); // #todo Make this customizable
//			break;
//		}
//
//		ObjectBase::SetPosition(hvn3::Math::Geometry::PointInDirection(Position(), to, max_speed));
//
//		// Transform the current view according to camera parameters.
//
//		View* view = _getView();
//
//		if (view == nullptr)
//			return;
//
//		// Rotate the view according to the horizontal speed of the camera (if tilting is enabled).
//
//		if (_tilt_enabled) {
//
//			view->SetAngle(view->Angle() + Math::Signum(to.x - X()) * (dist / 180.0f));
//			view->SetAngle(view->Angle() * 0.95f);
//
//		}
//
//		// Transform the view according to the shake parameters.
//
//		if (_shake_duration < _shake_duration_max) {
//
//			// #todo It's slow to construct a PerlinNoise instance repeatedly. Static or implementation change?
//			PerlinNoise gen;
//			gen.SetScale(-1.0f, 1.0f);
//
//			float intensity = ((_shake_duration_max - _shake_duration) / _shake_duration_max);
//			float angle_off = _shake_max_angle * intensity * gen.Noise(_shake_seed, _shake_duration);
//			float x_off = _shake_max_offset * intensity * gen.Noise(_shake_seed + 1.0f, _shake_duration);
//			float y_off = _shake_max_offset * intensity * gen.Noise(_shake_seed + 2.0f, _shake_duration);
//
//			view->SetAngle(angle_off);
//
//			ObjectBase::SetPosition(Position() + PointF(x_off, y_off));
//
//			_shake_duration += static_cast<float>(e.Delta());
//
//			if (_shake_duration >= _shake_duration_max) {
//
//				view->SetAngle(0.0f);
//				_shake_duration = 0.0f;
//				_shake_duration_max = 0.0f;
//
//			}
//
//		}
//
//	}
//	void Camera2d::OnContextChanged(ContextChangedEventArgs& e) {
//
//		_context = e.Context();
//
//	}
//	void Camera2d::SetPosition(const PointF& position) {
//
//		ObjectBase::SetPosition(position);
//
//		// Clear the target object when the position is set manually.
//		_target = nullptr;
//		_target_id = NoOne;
//
//	}
//	void Camera2d::SnapToTarget() {
//
//		ObjectBase::SetPosition(_getTargetPosition());
//
//	}
//	void Camera2d::SetFollowing(IObject* target) {
//
//		_target = target;
//
//		if (_target != nullptr)
//			_target_id = _target->Id();
//		else
//			_target_id = NoOne;
//
//	}
//	void Camera2d::SetFollowing(ObjectId target_id) {
//
//		if (_target_id == target_id)
//			return;
//
//		_target = nullptr;
//		_target_id = target_id;
//
//	}
//	void Camera2d::SetOffset(float offset_x, float offset_y) {
//		SetOffset(PointF(offset_x, offset_y));
//	}
//	void Camera2d::SetOffset(const PointF& offset) {
//		_offset = offset;
//	}
//	void Camera2d::SetPanMode(PanMode value) {
//		_pan_mode = value;
//	}
//	void Camera2d::SetTiltEnabled(bool value) {
//		_tilt_enabled = value;
//	}
//	void Camera2d::Shake(float max_offset, float max_angle, float duration) {
//
//		_shake_seed = Random::Get<float>();
//		_shake_max_offset = max_offset;
//		_shake_max_angle = max_angle;
//		_shake_duration = 0.0f;
//		_shake_duration_max = duration;
//
//	}
//
//	PointF Camera2d::_getTargetPosition() {
//
//		if (_hasFollowingTarget() && _context) {
//
//			auto& object_manager = _context.Get<OBJECT_MANAGER>();
//			IObject* ptr = nullptr;
//
//			// We need to make sure that the object still exists before trying to access it.
//
//			// Start by trying to find the object by its address.
//
//			if (_target != nullptr) {
//
//				object_manager.ForEach([&](const IObjectPtr& i) {
//
//					if (_target == i.get())
//						ptr = i.get();
//
//				});
//
//			}
//
//			// If we didn't find the object, it no longer exists.
//			// If we have an ID to work with, follow the next object with that ID.
//
//			if (ptr == nullptr && _target_id != NoOne) {
//
//				while (ptr = object_manager.FindNext(_target_id), ptr != nullptr) {
//
//					_target = ptr;
//					break;
//
//				}
//
//			}
//
//			if (ptr != nullptr)
//				return ptr->Position();
//
//		}
//
//		// If we didn't find the object, there's nothing to follow, so keep the current position.
//		return Position();
//
//	}
//	bool Camera2d::_hasFollowingTarget() {
//		return !(_target_id == NoOne && _target == nullptr);
//	}
//	View* Camera2d::_getView() {
//
//		auto& view_manager = _context.Get<VIEW_MANAGER>();
//
//		for (size_t i = 0; i < view_manager.Count(); ++i)
//			if (view_manager.At(i).GetFollowing() == this)
//				return &view_manager.At(i);
//
//		return nullptr;
//
//	}
//
//}