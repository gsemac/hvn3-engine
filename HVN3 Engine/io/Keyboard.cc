#include <allegro5/allegro.h>
#include "Keyboard.h"

namespace hvn3 {

	Keyboard::Key Keyboard::__key_states[ALLEGRO_KEY_MAX];
	bool Keyboard::__pressed_any = false;
	bool Keyboard::released_any = false;
	int Keyboard::__held_count = 0;
	int Keyboard::__last_char = 0;
	int Keyboard::__last_key = 0;

	Keyboard::Keyboard() {}
	bool Keyboard::KeyDown(int key_code) {

		switch (key_code) {
		case KEY_ANY: return (__held_count > 0);
		case KEY_SHIFT: return (__key_states[ALLEGRO_KEY_LSHIFT].held || __key_states[ALLEGRO_KEY_RSHIFT].held);
		case KEY_CONTROL: return (__key_states[ALLEGRO_KEY_LCTRL].held || __key_states[ALLEGRO_KEY_RCTRL].held);
		default: return __key_states[key_code].held;
		}

	}
	bool Keyboard::KeyPressed(int key_code) {

		switch (key_code) {
		case KEY_ANY: return __pressed_any;
		case KEY_SHIFT: return (__key_states[ALLEGRO_KEY_LSHIFT].pressed || __key_states[ALLEGRO_KEY_RSHIFT].pressed);
		case KEY_CONTROL: return (__key_states[ALLEGRO_KEY_LCTRL].pressed || __key_states[ALLEGRO_KEY_RCTRL].pressed);
		default: return __key_states[key_code].pressed;
		}

	}
	bool Keyboard::KeyPressedOrHeld(int key_code) {

		return Keyboard::KeyDown(key_code) || Keyboard::KeyPressed(key_code);

	}
	bool Keyboard::KeyReleased(int key_code) {

		switch (key_code) {
		case KEY_ANY: return released_any;
		case KEY_SHIFT: return (__key_states[ALLEGRO_KEY_LSHIFT].released || __key_states[ALLEGRO_KEY_RSHIFT].released);
		case KEY_CONTROL: return (__key_states[ALLEGRO_KEY_LCTRL].released || __key_states[ALLEGRO_KEY_RCTRL].released);
		default: return __key_states[key_code].released;
		}

	}
	int Keyboard::LastChar() {

		return __last_char;

	}
	void Keyboard::ClearLastChar() {

		__last_char = 0;

	}
	int Keyboard::LastKey() {

		return __last_key;

	}
	void Keyboard::ClearLastKey() {

		__last_key = 0;

	}
	bool Keyboard::IsChar(int key_code) {

		return (key_code >= 1 && key_code < 47) || // A-Z, 0-9
			(key_code >= 60 && key_code < 63) || // ~ - =
			(key_code >= 65 && key_code < 67) || // [ ]
			(key_code >= 68 && key_code < 76) || // ; " \ , . / 
			(key_code >= 86 && key_code < 89); // / * - + (keypad)

	}
	hvn3::EventSource Keyboard::EventSource() {

		return al_get_keyboard_event_source();

	}

	void Keyboard::StateAccessor::ResetKeyStates() {

		for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
			__key_states[i].pressed = false;
			__key_states[i].released = false;
			__key_states[i].held = false;
		}

	}
	void Keyboard::StateAccessor::SetKeyState(int key_code, bool pressed) {

		__key_states[key_code].held = pressed;

		if (!__key_states[key_code].locked)
			__key_states[key_code].pressed = pressed;
		__key_states[key_code].released = !pressed;

		__key_states[key_code].locked = pressed;

		if (__key_states[key_code].held)
			++__held_count;
		if (__key_states[key_code].pressed)
			__pressed_any = true;
		else if (__key_states[key_code].released) {
			--__held_count;
			released_any = true;
		}

		if (pressed)
			__last_key = key_code;

	}
	void Keyboard::StateAccessor::ResetKeyStates(bool pressed, bool released, bool held) {

		for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
			__key_states[i].pressed = __key_states[i].pressed && !pressed;
			__key_states[i].released = __key_states[i].released && !released;
			__key_states[i].held = __key_states[i].held && !held;
		}

		__pressed_any = __pressed_any && !pressed;
		released_any = released_any && !released;

	}
	void Keyboard::StateAccessor::SetLastChar(int key_char) {

		__last_char = key_char;

	}

}