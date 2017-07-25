#pragma once
#include "EventArgs.h"
#include "io/Keyboard.h"

namespace hvn3 {

	class KeyEventArgs : public System::EventArgs {

	public:
		KeyEventArgs(Key key);

		Key Key() const;
		int KeyCode() const;
		KeyModifiers Modifiers() const;

	private:
		hvn3::Key _key;
		KeyModifiers _modifiers;

	};

	class KeyDownEventArgs : public KeyEventArgs {

	public:
		using KeyEventArgs::KeyEventArgs;

	};

	class KeyPressedEventArgs : public KeyEventArgs {

	public:
		using KeyEventArgs::KeyEventArgs;

	};

	class KeyReleasedEventArgs : public KeyEventArgs {

	public:
		using KeyEventArgs::KeyEventArgs;

	};

	class KeyCharEventArgs : public KeyEventArgs {

	public:
		KeyCharEventArgs(hvn3::Key key, int keychar);

		char Char() const;
		int CharCode() const;

	private:
		int _keychar;

	};

}