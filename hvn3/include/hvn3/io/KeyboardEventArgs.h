#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/io/Keyboard.h"

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

	class KeyboardLostEventArgs : public System::EventArgs {};
	class KeyboardFoundEventArgs : public System::EventArgs {};

}