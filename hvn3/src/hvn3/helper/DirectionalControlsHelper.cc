#include "hvn3/math/MathUtils.h"
#include "hvn3/helper/DirectionalControlsHelper.h"
#define KEY_COUNT (sizeof(_keys) / sizeof(_keys[0]))

namespace hvn3 {

	DirectionalControlsHelper::DirectionalControlsHelper(int directions, float speed) {
		_max_speed = speed;
		_acceleration = speed;
		_friction = speed;
		_directions = directions;
		_last_hdir = DIRECTION_NONE;
		_last_vdir = DIRECTION_NONE;
		SetKeys(Key::Up, Key::Left, Key::Down, Key::Right);
	}

	void DirectionalControlsHelper::OnKeyDown(KeyDownEventArgs& e) {}
	void DirectionalControlsHelper::OnKeyPressed(KeyPressedEventArgs& e) {

		if (_directions <= 0)
			return;

		if (e.Key() == _keys[KEYDIR_LEFT].key) {
			_keys[KEYDIR_LEFT].pressed = true;
			_last_hdir = DIRECTION_LEFT;
			if (_directions <= 4)
				_last_vdir = DIRECTION_NONE;
		}
		else if (e.Key() == _keys[KEYDIR_RIGHT].key) {
			_keys[KEYDIR_RIGHT].pressed = true;
			_last_hdir = DIRECTION_RIGHT;
			if (_directions <= 4)
				_last_vdir = DIRECTION_NONE;
		}
		else if (_directions > 2) {
			if (e.Key() == _keys[KEYDIR_UP].key) {
				_keys[KEYDIR_UP].pressed = true;
				_last_vdir = DIRECTION_UP;
				if (_directions <= 4)
					_last_hdir = DIRECTION_NONE;
			}
			else if (e.Key() == _keys[KEYDIR_DOWN].key) {
				_keys[KEYDIR_DOWN].pressed = true;
				_last_vdir = DIRECTION_DOWN;
				if (_directions <= 4)
					_last_hdir = DIRECTION_NONE;
			}
		}

		_updateLastDirection();

	}
	void DirectionalControlsHelper::OnKeyUp(KeyUpEventArgs& e) {

		if (e.Key() == _keys[KEYDIR_LEFT].key) {
			_keys[KEYDIR_LEFT].pressed = false;
			if (_keys[KEYDIR_RIGHT].pressed)
				_last_hdir = DIRECTION_RIGHT;
			else
				_last_hdir = DIRECTION_NONE;
		}
		else if (e.Key() == _keys[KEYDIR_RIGHT].key) {
			_keys[KEYDIR_RIGHT].pressed = false;
			if (_keys[KEYDIR_LEFT].pressed)
				_last_hdir = DIRECTION_LEFT;
			else
				_last_hdir = DIRECTION_NONE;
		}
		else if (e.Key() == _keys[KEYDIR_UP].key) {
			_keys[KEYDIR_UP].pressed = false;
			if (_keys[KEYDIR_DOWN].pressed)
				_last_vdir = DIRECTION_DOWN;
			else
				_last_vdir = DIRECTION_NONE;
		}
		else if (e.Key() == _keys[KEYDIR_DOWN].key) {
			_keys[KEYDIR_DOWN].pressed = false;
			if (_keys[KEYDIR_UP].pressed)
				_last_vdir = DIRECTION_UP;
			else
				_last_vdir = DIRECTION_NONE;
		}

		_updateLastDirection();

	}

	float DirectionalControlsHelper::Acceleration() const {
		return _acceleration;
	}
	void DirectionalControlsHelper::SetAcceleration(float value) {
		_acceleration = value;
	}
	float DirectionalControlsHelper::Friction() const {
		return _friction;
	}
	void DirectionalControlsHelper::SetFriction(float value) {
		_friction = value;
	}
	float DirectionalControlsHelper::MaxSpeed() const {
		return _max_speed;
	}
	void DirectionalControlsHelper::SetMaxSpeed(float value) {
		_max_speed = value;
	}
	const Vector2d& DirectionalControlsHelper::Velocity() const {
		return _velocity;
	}
	void DirectionalControlsHelper::SetKeys(Key up, Key left, Key down, Key right) {

		_keys[KEYDIR_UP].key = up;
		_keys[KEYDIR_DOWN].key = down;
		_keys[KEYDIR_LEFT].key = left;
		_keys[KEYDIR_RIGHT].key = right;

		_resetKeyStates();

	}
	Direction8 DirectionalControlsHelper::Direction() const {

		return Direction8(_last_hdir, _last_vdir);

	}
	Direction8 DirectionalControlsHelper::LastDirection() const {

		return _last_dir;

	}

	void DirectionalControlsHelper::Step() {
	
		// If no keys are pressed, reset the direction and keystates.
		// This is done just in case the key release event is missed (e.g., released while display is unfocused).
		for (int i = 0; i < KEY_COUNT; ++i) {
			if (Keyboard::KeyDown(_keys[i].key))
				break;
			if (i + 1 == KEY_COUNT)
				_resetKeyStates();
		}

		if (_directions <= 0)
			return;

		if (_last_hdir == DIRECTION_LEFT) {
			if (_velocity.X() > -_max_speed)
				_velocity.SetX(Math::Max(_velocity.X() - _acceleration, -_max_speed));
		}
		else if (_last_hdir == DIRECTION_RIGHT) {
			if (_velocity.X() < _max_speed)
				_velocity.SetX(Math::Min(_velocity.X() + _acceleration, _max_speed));
		}
		else if (_last_hdir == DIRECTION_NONE && _velocity.X() != 0.0f) {
			if (_velocity.X() < 0.0f)
				_velocity.SetX(Math::Min(_velocity.X() + _friction, 0.0f));
			else if (_velocity.X() > 0.0f)
				_velocity.SetX(Math::Max(_velocity.X() - _friction, 0.0f));
		}

		if (_directions <= 2)
			return;

		if (_last_vdir == DIRECTION_UP) {
			if (_velocity.Y() > -_max_speed)
				_velocity.SetY(Math::Max(_velocity.Y() - _acceleration, -_max_speed));
		}
		else if (_last_vdir == DIRECTION_DOWN) {
			if (_velocity.Y() < _max_speed)
				_velocity.SetY(Math::Min(_velocity.Y() + _acceleration, _max_speed));
		}
		else if (_last_vdir == DIRECTION_NONE && _velocity.Y() != 0.0f) {
			if (_velocity.Y() < 0.0f)
				_velocity.SetY(Math::Min(_velocity.Y() + _friction, 0.0f));
			else if (_velocity.Y() > 0.0f)
				_velocity.SetY(Math::Max(_velocity.Y() - _friction, 0.0f));
		}

	}
	void DirectionalControlsHelper::Clear() {
		_velocity.SetX(0.0f);
		_velocity.SetY(0.0f);
	}



	DirectionalControlsHelper::KeyData DirectionalControlsHelper::GetKeyData(KEYDIR keydir) {
		return _keys[keydir];
	}
	void DirectionalControlsHelper::SetVelocity(const Vector2d& value) {
		_velocity = value;
	}



	void DirectionalControlsHelper::_resetKeyStates() {
		_keys[KEYDIR_UP].pressed = false;
		_keys[KEYDIR_DOWN].pressed = false;
		_keys[KEYDIR_LEFT].pressed = false;
		_keys[KEYDIR_RIGHT].pressed = false;
		_last_hdir = DIRECTION_NONE;
		_last_vdir = DIRECTION_NONE;
	}
	void DirectionalControlsHelper::_updateLastDirection() {
		DIRECTION last_hdir = _last_hdir != DIRECTION_NONE ? _last_hdir : _last_dir.HorizontalDirection();
		DIRECTION last_vdir = _last_vdir != DIRECTION_NONE ? _last_vdir : _last_dir.VerticalDirection();
		_last_dir = Direction8(last_hdir, last_vdir);
	}

}