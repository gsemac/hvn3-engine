#include "hvn3/math/MathUtils.h"
#include "hvn3/io/DirectionalKeyboardController.h"
#define KEY_COUNT (sizeof(_keys) / sizeof(_keys[0]))

namespace hvn3 {

	DirectionalKeyboardController::DirectionalKeyboardController(int directions, float speed) {
		_max_speed = speed;
		_acceleration = speed;
		_friction = speed;
		_directions = directions;
		_last_hdir = DIRECTION_NONE;
		_last_vdir = DIRECTION_NONE;
		SetKeys(Key::Left, Key::Right, Key::Up, Key::Down);
	}

	void DirectionalKeyboardController::OnKeyDown(KeyDownEventArgs& e) {}
	void DirectionalKeyboardController::OnKeyPressed(KeyPressedEventArgs& e) {

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

	}
	void DirectionalKeyboardController::OnKeyUp(KeyUpEventArgs& e) {

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

	}

	float DirectionalKeyboardController::Acceleration() const {
		return _acceleration;
	}
	void DirectionalKeyboardController::SetAcceleration(float value) {
		_acceleration = value;
	}
	float DirectionalKeyboardController::Friction() const {
		return _friction;
	}
	void DirectionalKeyboardController::SetFriction(float value) {
		_friction = value;
	}
	float DirectionalKeyboardController::MaxSpeed() const {
		return _max_speed;
	}
	void DirectionalKeyboardController::SetMaxSpeed(float value) {
		_max_speed = value;
	}
	const Vector2d& DirectionalKeyboardController::Velocity() const {
		return _velocity;
	}
	void DirectionalKeyboardController::SetKeys(Key left, Key right, Key up, Key down) {

		_keys[KEYDIR_UP].key = up;
		_keys[KEYDIR_DOWN].key = down;
		_keys[KEYDIR_LEFT].key = left;
		_keys[KEYDIR_RIGHT].key = right;

		_resetKeyStates();

	}

	void DirectionalKeyboardController::Step() {
	
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
	void DirectionalKeyboardController::Clear() {
		_velocity.SetX(0.0f);
		_velocity.SetY(0.0f);
	}



	DirectionalKeyboardController::KeyData DirectionalKeyboardController::GetKeyData(KEYDIR keydir) {
		return _keys[keydir];
	}
	void DirectionalKeyboardController::SetVelocity(const Vector2d& value) {
		_velocity = value;
	}



	void DirectionalKeyboardController::_resetKeyStates() {
		_keys[KEYDIR_UP].pressed = false;
		_keys[KEYDIR_DOWN].pressed = false;
		_keys[KEYDIR_LEFT].pressed = false;
		_keys[KEYDIR_RIGHT].pressed = false;
		_last_hdir = DIRECTION_NONE;
		_last_vdir = DIRECTION_NONE;
	}

}