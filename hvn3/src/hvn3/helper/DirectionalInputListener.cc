#include "hvn3/math/MathUtils.h"
#include "hvn3/helper/DirectionalInputListener.h"

#define KEY_COUNT (sizeof(_keys) / sizeof(_keys[0]))

namespace hvn3 {

	DirectionalInputListener::DirectionalInputListener(int directions) {

		_directions = directions;
		_last_hdir = DIRECTION_NONE;
		_last_vdir = DIRECTION_NONE;

		SetKeys(Key::Up, Key::Left, Key::Down, Key::Right);

	}
	void DirectionalInputListener::OnKeyPressed(KeyPressedEventArgs& e) {

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
	void DirectionalInputListener::OnKeyUp(KeyUpEventArgs& e) {

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
	void DirectionalInputListener::OnKeyboardLost(KeyboardLostEventArgs& e) {

		// If no keys are pressed, reset the direction and keystates.
		// This is done just in case the key release event is missed (e.g., released while display is unfocused).
		for (int i = 0; i < KEY_COUNT; ++i) {

			if (Keyboard::KeyDown(_keys[i].key))
				break;

			if (i + 1 == KEY_COUNT)
				_resetKeyStates();

		}

	}
	void DirectionalInputListener::SetKeys(Key up, Key left, Key down, Key right) {

		_keys[KEYDIR_UP].key = up;
		_keys[KEYDIR_DOWN].key = down;
		_keys[KEYDIR_LEFT].key = left;
		_keys[KEYDIR_RIGHT].key = right;

		_resetKeyStates();

	}
	Direction8 DirectionalInputListener::Direction() const {
		return Direction8(_last_hdir, _last_vdir);
	}
	Direction8 DirectionalInputListener::LastDirection() const {
		return _last_dir;
	}

	DirectionalInputListener::KeyData DirectionalInputListener::GetKeyData(KEYDIR keydir) {
		return _keys[keydir];
	}
	int DirectionalInputListener::DirectionCount() const {
		return _directions;
	}

	void DirectionalInputListener::_resetKeyStates() {
		
		_keys[KEYDIR_UP].pressed = false;
		_keys[KEYDIR_DOWN].pressed = false;
		_keys[KEYDIR_LEFT].pressed = false;
		_keys[KEYDIR_RIGHT].pressed = false;
		
		_last_hdir = DIRECTION_NONE;
		_last_vdir = DIRECTION_NONE;

	}
	void DirectionalInputListener::_updateLastDirection() {
		
		DIRECTION last_hdir = _last_hdir != DIRECTION_NONE ? _last_hdir : _last_dir.HorizontalDirection();
		DIRECTION last_vdir = _last_vdir != DIRECTION_NONE ? _last_vdir : _last_dir.VerticalDirection();
		
		_last_dir = Direction8(last_hdir, last_vdir);

	}

}