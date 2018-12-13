#pragma once
#include "hvn3/events/EventSource.h"
#include "hvn3/io/IKeyboardListener.h"
#include "hvn3/io/IODefs.h"
#include "hvn3/io/ListenerBase.h"
#include "hvn3/utility/BitFlags.h"

#include <queue>
#include <string>

namespace hvn3 {

	class GameManager;

	namespace System {
		class KeyboardMutator;
	}

	class Keyboard {
		friend class System::KeyboardMutator;

	public:
		typedef ListenerCollection<IKeyboardListener> Listeners;

		static bool KeyPressed(int key_code);
		static bool KeyDown(int key_code);
		static bool KeyReleased(int key_code);
		static bool KeyPressedOrHeld(int key_code);
		static bool KeyPressed(hvn3::Key key);
		static bool KeyDown(hvn3::Key key);
		static bool KeyReleased(hvn3::Key key);
		static bool KeyPressedOrHeld(hvn3::Key key);

		static int LastChar();
		static int LastKey();
		static void ClearLastChar();
		static void ClearLastKey();
		static bool IsPrintableChar(int key_code);
		static bool IsPrintableChar(hvn3::Key key);
		static bool IsNumeric(hvn3::Key key);

	private:
		struct Key {

			bool held; // whether or not key is held
			bool pressed; // whether or not key is pressed (instantaneous)
			bool locked; // if locked, pressed is false until released and pressed again (for single key presses)
			bool released; // whether or not the key was released (instaneous)

			Key() : held(0), pressed(0), locked(0), released(0) {}

		};

		static Key _key_states[];
		static bool _pressed_any, _released_any;
		static int _held_count;
		static int _last_char;
		static int _last_key;
		static int _modifier;

		Keyboard() = default;

	};

}