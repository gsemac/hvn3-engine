#include <allegro5/allegro.h>
#include "Keyboard.h"

namespace hvn3 {

	Keyboard::Key Keyboard::_key_states[ALLEGRO_KEY_MAX];
	bool Keyboard::_pressed_any = false;
	bool Keyboard::_released_any = false;
	int Keyboard::_held_count = 0;
	int Keyboard::_last_char = 0;
	int Keyboard::_last_key = 0;

	Keyboard::Keyboard() {}

	bool Keyboard::KeyDown(int key_code) {

		switch (key_code) {
		case (int)hvn3::Key::Any: return (_held_count > 0);
		case (int)hvn3::Key::Shift: return (_key_states[ALLEGRO_KEY_LSHIFT].held || _key_states[ALLEGRO_KEY_RSHIFT].held);
		case (int)hvn3::Key::Control: return (_key_states[ALLEGRO_KEY_LCTRL].held || _key_states[ALLEGRO_KEY_RCTRL].held);
		default: return _key_states[key_code].held;
		}

	}
	bool Keyboard::KeyPressed(int key_code) {

		switch (key_code) {
		case (int)hvn3::Key::Any: return _pressed_any;
		case (int)hvn3::Key::Shift: return (_key_states[ALLEGRO_KEY_LSHIFT].pressed || _key_states[ALLEGRO_KEY_RSHIFT].pressed);
		case (int)hvn3::Key::Control: return (_key_states[ALLEGRO_KEY_LCTRL].pressed || _key_states[ALLEGRO_KEY_RCTRL].pressed);
		default: return _key_states[key_code].pressed;
		}

	}
	bool Keyboard::KeyPressedOrHeld(int key_code) {

		return Keyboard::KeyDown(key_code) || Keyboard::KeyPressed(key_code);

	}
	bool Keyboard::KeyReleased(int key_code) {

		switch (key_code) {
		case (int)hvn3::Key::Any: return _released_any;
		case (int)hvn3::Key::Shift:	return (_key_states[ALLEGRO_KEY_LSHIFT].released || _key_states[ALLEGRO_KEY_RSHIFT].released);
		case (int)hvn3::Key::Control: return (_key_states[ALLEGRO_KEY_LCTRL].released || _key_states[ALLEGRO_KEY_RCTRL].released);
		default: return _key_states[key_code].released;
		}

	}

	bool Keyboard::KeyPressed(hvn3::Key key) {

		return KeyPressed((int)key);

	}
	bool Keyboard::KeyDown(hvn3::Key key) {

		return KeyDown((int)key);

	}
	bool Keyboard::KeyReleased(hvn3::Key key) {

		return KeyReleased((int)key);

	}
	bool Keyboard::KeyPressedOrHeld(hvn3::Key key) {

		return KeyPressedOrHeld((int)key);

	}

	int Keyboard::LastChar() {

		return _last_char;

	}
	void Keyboard::ClearLastChar() {

		_last_char = 0;

	}
	int Keyboard::LastKey() {

		return _last_key;

	}
	void Keyboard::ClearLastKey() {

		_last_key = 0;

	}
	bool Keyboard::IsChar(int key_code) {

		return (key_code >= 1 && key_code < 47) || // A-Z, 0-9
			(key_code >= 60 && key_code < 63) || // ~ - =
			(key_code >= 65 && key_code < 67) || // [ ]
			(key_code >= 68 && key_code < 76) || // ; " \ , . / 
			(key_code >= 86 && key_code < 89); // / * - + (keypad)

	}
	bool Keyboard::IsChar(hvn3::Key key) {

		return IsChar((int)key);

	}

	hvn3::EventSource Keyboard::EventSource() {

		return al_get_keyboard_event_source();

	}

	void Keyboard::StateAccessor::ResetKeyStates() {

		for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
			_key_states[i].pressed = false;
			_key_states[i].released = false;
			_key_states[i].held = false;
		}

	}
	void Keyboard::StateAccessor::SetKeyState(int key_code, bool pressed) {

		_key_states[key_code].held = pressed;

		if (!_key_states[key_code].locked)
			_key_states[key_code].pressed = pressed;
		_key_states[key_code].released = !pressed;

		_key_states[key_code].locked = pressed;

		if (_key_states[key_code].held)
			++_held_count;
		if (_key_states[key_code].pressed)
			_pressed_any = true;
		else if (_key_states[key_code].released) {
			--_held_count;
			_released_any = true;
		}

		if (pressed)
			_last_key = key_code;

	}
	void Keyboard::StateAccessor::ResetKeyStates(bool pressed, bool released, bool held) {

		for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
			_key_states[i].pressed = _key_states[i].pressed && !pressed;
			_key_states[i].released = _key_states[i].released && !released;
			_key_states[i].held = _key_states[i].held && !held;
		}

		_pressed_any = _pressed_any && !pressed;
		_released_any = _released_any && !released;

	}
	void Keyboard::StateAccessor::SetLastChar(int key_char) {

		_last_char = key_char;

	}

}