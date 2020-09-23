#include "hvn3/io/KeyboardEventArgs.h"

namespace hvn3 {

	KeyboardEventArgs::KeyboardEventArgs(hvn3::Key key, KeyModifiers modifiers) {

		_key = key;
		_modifiers = modifiers;

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

	KeyCharEventArgs::KeyCharEventArgs(hvn3::Key key, KeyModifiers modifiers, int keychar) :
		KeyboardEventArgs(key, modifiers) {

		_keychar = keychar;

	}
	char KeyCharEventArgs::Char() const {

		return (char)_keychar;

	}
	int KeyCharEventArgs::CharCode() const {

		return _keychar;

	}
}