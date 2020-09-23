//#include "hvn3/math/MathUtils.h"
//#include "hvn3/helper/DirectionalControlsHelper.h"
//
//namespace hvn3 {
//
//	DirectionalControlsHelper::DirectionalControlsHelper(int directions, float speed) :
//		DirectionalInputListener(directions) {
//		
//		_max_speed = speed;
//		_acceleration = speed;
//		_friction = speed;
//
//		SetKeys(Key::Up, Key::Left, Key::Down, Key::Right);
//
//	}
//
//	float DirectionalControlsHelper::Acceleration() const {
//		return _acceleration;
//	}
//	void DirectionalControlsHelper::SetAcceleration(float value) {
//		_acceleration = value;
//	}
//	float DirectionalControlsHelper::Friction() const {
//		return _friction;
//	}
//	void DirectionalControlsHelper::SetFriction(float value) {
//		_friction = value;
//	}
//	float DirectionalControlsHelper::MaxSpeed() const {
//		return _max_speed;
//	}
//	void DirectionalControlsHelper::SetMaxSpeed(float value) {
//		_max_speed = value;
//	}
//	const Vector2d& DirectionalControlsHelper::Velocity() const {
//		return _velocity;
//	}
//	void DirectionalControlsHelper::Step() {
//
//		if (DirectionCount() <= 0)
//			return;
//
//		DIRECTION hdir = Direction().HorizontalDirection();
//		DIRECTION vdir = Direction().VerticalDirection();
//
//		if (hdir == DIRECTION_LEFT) {
//			if (_velocity.X() > -_max_speed)
//				_velocity.SetX(Math::Max(_velocity.X() - _acceleration, -_max_speed));
//		}
//		else if (hdir == DIRECTION_RIGHT) {
//			if (_velocity.X() < _max_speed)
//				_velocity.SetX(Math::Min(_velocity.X() + _acceleration, _max_speed));
//		}
//		else if (hdir == DIRECTION_NONE && _velocity.X() != 0.0f) {
//			if (_velocity.X() < 0.0f)
//				_velocity.SetX(Math::Min(_velocity.X() + _friction, 0.0f));
//			else if (_velocity.X() > 0.0f)
//				_velocity.SetX(Math::Max(_velocity.X() - _friction, 0.0f));
//		}
//
//		if (DirectionCount() <= 2)
//			return;
//
//		if (vdir == DIRECTION_UP) {
//			if (_velocity.Y() > -_max_speed)
//				_velocity.SetY(Math::Max(_velocity.Y() - _acceleration, -_max_speed));
//		}
//		else if (vdir == DIRECTION_DOWN) {
//			if (_velocity.Y() < _max_speed)
//				_velocity.SetY(Math::Min(_velocity.Y() + _acceleration, _max_speed));
//		}
//		else if (vdir == DIRECTION_NONE && _velocity.Y() != 0.0f) {
//			if (_velocity.Y() < 0.0f)
//				_velocity.SetY(Math::Min(_velocity.Y() + _friction, 0.0f));
//			else if (_velocity.Y() > 0.0f)
//				_velocity.SetY(Math::Max(_velocity.Y() - _friction, 0.0f));
//		}
//
//	}
//	void DirectionalControlsHelper::Clear() {
//		_velocity.SetX(0.0f);
//		_velocity.SetY(0.0f);
//	}
//
//	void DirectionalControlsHelper::SetVelocity(const Vector2d& value) {
//		_velocity = value;
//	}
//
//}