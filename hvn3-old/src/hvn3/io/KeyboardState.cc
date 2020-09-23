#include "hvn3/io/KeyboardState.h"

namespace hvn3 {

	// KeyboardState

	// Public methods

	KeyboardState::KeyboardState() :
		_pressed_any(false),
		_released_any(false),
		_held_count(0),
		_last_char(0),
		_last_key(0),
		_modifier(0) {

		_key_states.fill(KeyState());

	}

	bool KeyboardState::KeyDown(int keyCode) {

		switch (keyCode) {
		case (int)hvn3::Key::Any: return (_held_count > 0);
		case (int)hvn3::Key::Shift: return (_key_states[ALLEGRO_KEY_LSHIFT].held || _key_states[ALLEGRO_KEY_RSHIFT].held);
		case (int)hvn3::Key::Control: return (_key_states[ALLEGRO_KEY_LCTRL].held || _key_states[ALLEGRO_KEY_RCTRL].held);
		case (int)hvn3::Key::Alt: return (_key_states[ALLEGRO_KEY_ALT].held || _key_states[ALLEGRO_KEY_ALTGR].held);
		default: return _key_states[keyCode].held;
		}

	}
	bool KeyboardState::KeyPressed(int keyCode) {

		switch (keyCode) {
		case (int)hvn3::Key::Any: return _pressed_any;
		case (int)hvn3::Key::Shift: return (_key_states[ALLEGRO_KEY_LSHIFT].pressed || _key_states[ALLEGRO_KEY_RSHIFT].pressed);
		case (int)hvn3::Key::Control: return (_key_states[ALLEGRO_KEY_LCTRL].pressed || _key_states[ALLEGRO_KEY_RCTRL].pressed);
		case (int)hvn3::Key::Alt: return (_key_states[ALLEGRO_KEY_ALT].pressed || _key_states[ALLEGRO_KEY_ALTGR].pressed);
		default: return _key_states[keyCode].pressed;
		}

	}
	bool KeyboardState::KeyPressedOrHeld(int keyCode) {

		return KeyDown(keyCode) || KeyPressed(keyCode);

	}
	bool KeyboardState::KeyReleased(int keyCode) {

		switch (keyCode) {
		case (int)hvn3::Key::Any: return _released_any;
		case (int)hvn3::Key::Shift:	return (_key_states[ALLEGRO_KEY_LSHIFT].released || _key_states[ALLEGRO_KEY_RSHIFT].released);
		case (int)hvn3::Key::Control: return (_key_states[ALLEGRO_KEY_LCTRL].released || _key_states[ALLEGRO_KEY_RCTRL].released);
		case (int)hvn3::Key::Alt: return (_key_states[ALLEGRO_KEY_ALT].released || _key_states[ALLEGRO_KEY_ALTGR].released);
		default: return _key_states[keyCode].released;
		}

	}

	bool KeyboardState::KeyPressed(Key key) {

		return KeyPressed((int)key);

	}
	bool KeyboardState::KeyDown(Key key) {

		return KeyDown((int)key);

	}
	bool KeyboardState::KeyReleased(Key key) {

		return KeyReleased((int)key);

	}
	bool KeyboardState::KeyPressedOrHeld(Key key) {

		return KeyPressedOrHeld((int)key);

	}

	int KeyboardState::LastChar() {

		return _last_char;

	}
	void KeyboardState::ClearLastChar() {

		_last_char = 0;

	}
	int KeyboardState::LastKey() {

		return _last_key;

	}
	KeyModifiers KeyboardState::Modifiers() {

		KeyModifiers modifiers = static_cast<KeyModifiers>(0);

		if (KeyDown(Key::LShift))
			modifiers |= KeyModifiers::LShift;
		if (KeyDown(Key::RShift))
			modifiers |= KeyModifiers::RShift;
		if (KeyDown(Key::LControl))
			modifiers |= KeyModifiers::LControl;
		if (KeyDown(Key::RControl))
			modifiers |= KeyModifiers::RControl;
		if (KeyDown(Key::LAlt))
			modifiers |= KeyModifiers::RAlt;

		return modifiers;

	}
	void KeyboardState::ClearLastKey() {

		_last_key = 0;

	}

	void KeyboardState::ClearKeyStates(bool pressed, bool released, bool held) {

		for (size_t i = 0; i < _key_states.size(); ++i) {

			_key_states[i].pressed = _key_states[i].pressed && !pressed;
			_key_states[i].released = _key_states[i].released && !released;
			_key_states[i].held = _key_states[i].held && !held;

		}

		_pressed_any = _pressed_any && !pressed;
		_released_any = _released_any && !released;

	}
	void KeyboardState::ClearKeyStates() {

		for (size_t i = 0; i < _key_states.size(); ++i) {

			_key_states[i].pressed = false;
			_key_states[i].released = false;
			_key_states[i].held = false;

		}

	}
	void KeyboardState::SetKeyState(int keyCode, bool pressed) {

		_key_states[keyCode].held = pressed;

		if (!_key_states[keyCode].locked)
			_key_states[keyCode].pressed = pressed;

		_key_states[keyCode].released = !pressed;

		_key_states[keyCode].locked = pressed;

		if (_key_states[keyCode].held)
			++_held_count;

		if (_key_states[keyCode].pressed)
			_pressed_any = true;
		else if (_key_states[keyCode].released) {

			--_held_count;
			_released_any = true;

		}

		if (pressed)
			_last_key = keyCode;

	}
	void KeyboardState::SetLastChar(int keyChar) {
		_last_char = keyChar;
	}

	KeyboardState::Iterator KeyboardState::begin() {
		return Iterator(this, 0);
	}
	KeyboardState::Iterator KeyboardState::end() {
		return Iterator(this, _key_states.size());
	}

	// KeyboardState::KeyState

	// Public methods

	KeyboardState::KeyState::KeyState() :
		held(false),
		pressed(false),
		locked(false),
		released(false) {}

	// KeyboardState::Iterator

	// Public methods

	KeyboardState::Iterator::Iterator(KeyboardState* state, size_t keyIndex) :
		_state(state),
		_key_index(keyIndex) {
	}

	bool KeyboardState::Iterator::operator==(const Iterator& other) {
		return _key_index == other._key_index;
	}
	bool KeyboardState::Iterator::operator!=(const Iterator& other) {
		return !(*this == other);
	}
	KeyboardState::Iterator& KeyboardState::Iterator::operator++() {

		++_key_index;

		return *this;

	}
	KeyboardState::Iterator KeyboardState::Iterator::operator++(int) {

		Iterator before = *this;

		++(*this);

		return before;

	}
	KeyboardState::Iterator::KeyStateWrapper& KeyboardState::Iterator::operator*() {

		_initKeyState();

		return _key_state;

	}
	KeyboardState::Iterator::KeyStateWrapper* KeyboardState::Iterator::operator->() {
		return &(**this);
	}
	const KeyboardState::Iterator::KeyStateWrapper& KeyboardState::Iterator::operator*() const {

		_initKeyState();

		return _key_state;

	}
	const KeyboardState::Iterator::KeyStateWrapper* KeyboardState::Iterator::operator->() const {
		return &(**this);
	}

	// Private methods 

	void KeyboardState::Iterator::_initKeyState() const {
		_key_state = KeyStateWrapper(_state->_key_states[_key_index], (Key)_key_index);
	}

	// KeyboardState::Iterator::KeyStateWrapper

	// Public methods

	KeyboardState::Iterator::KeyStateWrapper::KeyStateWrapper(KeyState state, hvn3::Key key) :
		_key(key),
		_state(state) {
	}

	bool KeyboardState::Iterator::KeyStateWrapper::Held() const {
		return _state.held;
	}
	bool KeyboardState::Iterator::KeyStateWrapper::Pressed() const {
		return _state.pressed;
	}
	bool KeyboardState::Iterator::KeyStateWrapper::Released() const {
		return _state.released;
	}
	Key KeyboardState::Iterator::KeyStateWrapper::Key() const {
		return _key;
	}

}