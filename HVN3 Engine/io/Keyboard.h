#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include <queue>
#include <string>
#include "EventSource.h"

class GameManager;

enum KEYS {
	KEY_CONTROL = -2,
	KEY_SHIFT = -1,
	KEY_ANY = 0
};

class Keyboard {
	friend class StateAccessor;

public:
	class StateAccessor {

	public:
		static void ResetKeyStates(bool pressed, bool released, bool held);
		static void ResetKeyStates();
		static void SetKeyState(int key_code, bool pressed);
		static void SetLastChar(int key_char);

	};

	static bool KeyPressed(int);
	static bool KeyDown(int);
	static bool KeyReleased(int);
	static bool KeyPressedOrHeld(int);

	static int LastChar();
	static int LastKey();
	static void ClearLastChar();
	static void ClearLastKey();
	static bool IsChar(int key_code);

	static EventSource EventSource();

private:
	struct Key {

		bool held; // whether or not key is held
		bool pressed; // whether or not key is pressed (instantaneous)
		bool locked; // if locked, pressed is false until released and pressed again (for single key presses)
		bool released; // whether or not the key was released (instaneous)

		Key() : held(0), pressed(0), locked(0), released(0) {}

	};

	static Key __key_states[];
	static bool __pressed_any, released_any;
	static int __held_count;
	static int __last_char;
	static int __last_key;
	static int __modifier;

	Keyboard();
		
};

#endif
