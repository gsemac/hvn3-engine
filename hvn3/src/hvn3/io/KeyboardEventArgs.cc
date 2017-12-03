#include "hvn3/io/KeyboardEventArgs.h"

namespace hvn3 {

	KeyboardEventArgs::KeyboardEventArgs(hvn3::Key key) {

		_key = key;

		_modifiers = (KeyModifiers)0;

		if (Keyboard::KeyDown(Key::LShift))
			_modifiers |= KeyModifiers::LShift;
		if (Keyboard::KeyDown(Key::RShift))
			_modifiers |= KeyModifiers::RShift;
		if (Keyboard::KeyDown(Key::LControl))
			_modifiers |= KeyModifiers::LControl;
		if (Keyboard::KeyDown(Key::RControl))
			_modifiers |= KeyModifiers::RControl;
		if (Keyboard::KeyDown(Key::LAlt))
			_modifiers |= KeyModifiers::RAlt;

	}
	Key KeyboardEventArgs::Key() const {

		return _key;

	}
	int KeyboardEventArgs::KeyCode() const {

		return (int)_key;

	}
	KeyModifiers KeyboardEventArgs::Modifiers() const {

		return _modifiers;

	}

	KeyCharEventArgs::KeyCharEventArgs(hvn3::Key key, int keychar) :
		KeyboardEventArgs(key) {

		_keychar = keychar;

	}
	char KeyCharEventArgs::Char() const {

		return (char)_keychar;

	}
	int KeyCharEventArgs::CharCode() const {

		return _keychar;

	}
}