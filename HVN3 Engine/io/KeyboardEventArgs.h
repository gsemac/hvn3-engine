#pragma once
#include "EventArgs.h"
#include "io/Keyboard.h"

namespace hvn3 {

	class KeyboardEventArgs : public System::EventArgs {

	public:
		KeyboardEventArgs(Key key);

		Key Key() const;
		int KeyCode() const;
		KeyModifiers Modifiers() const;

	private:
		hvn3::Key _key;
		KeyModifiers _modifiers;

	};

	class KeyDownEventArgs : public KeyboardEventArgs {

	public:
		using KeyboardEventArgs::KeyboardEventArgs;

	};

	class KeyPressedEventArgs : public KeyboardEventArgs {

	public:
		using KeyboardEventArgs::KeyboardEventArgs;

	};

	class KeyUpEventArgs : public KeyboardEventArgs {

	public:
		using KeyboardEventArgs::KeyboardEventArgs;

	};

	class KeyCharEventArgs : public KeyboardEventArgs {

	public:
		KeyCharEventArgs(hvn3::Key key, int keychar);

		char Char() const;
		int CharCode() const;

	private:
		int _keychar;

	};

}