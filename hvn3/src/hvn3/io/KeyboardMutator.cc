#include "hvn3/io/KeyboardMutator.h"
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/io/Keyboard.h"
#include <allegro5/allegro.h>

namespace hvn3 {
	namespace System {

		void KeyboardMutator::ResetKeyStates() const {

			for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
				Keyboard::_key_states[i].pressed = false;
				Keyboard::_key_states[i].released = false;
				Keyboard::_key_states[i].held = false;
			}

		}
		void KeyboardMutator::SetKeyState(int key_code, bool pressed) const {

			Keyboard::_key_states[key_code].held = pressed;

			if (!Keyboard::_key_states[key_code].locked)
				Keyboard::_key_states[key_code].pressed = pressed;
			Keyboard::_key_states[key_code].released = !pressed;

			Keyboard::_key_states[key_code].locked = pressed;

			if (Keyboard::_key_states[key_code].held)
				++Keyboard::_held_count;
			if (Keyboard::_key_states[key_code].pressed)
				Keyboard::_pressed_any = true;
			else if (Keyboard::_key_states[key_code].released) {
				--Keyboard::_held_count;
				Keyboard::_released_any = true;
			}

			if (pressed)
				Keyboard::_last_key = key_code;

		}
		void KeyboardMutator::ResetKeyStates(bool pressed, bool released, bool held) const {

			for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
				Keyboard::_key_states[i].pressed = Keyboard::_key_states[i].pressed && !pressed;
				Keyboard::_key_states[i].released = Keyboard::_key_states[i].released && !released;
				Keyboard::_key_states[i].held = Keyboard::_key_states[i].held && !held;
			}

			Keyboard::_pressed_any = Keyboard::_pressed_any && !pressed;
			Keyboard::_released_any = Keyboard::_released_any && !released;

		}
		void KeyboardMutator::SetLastChar(int key_char) const {

			Keyboard::_last_char = key_char;

		}

		void KeyboardMutator::DispatchAllKeyDownEvents() const {

			for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i)
				if (Keyboard::_key_states[i].held)
					DispatchEvent(KeyDownEventArgs((Key)i));

		}
		void KeyboardMutator::DispatchEvent(KeyDownEventArgs& e) const {

			for (auto i = KeyboardListener::_listeners().begin(); i != KeyboardListener::_listeners().end(); ++i)
				(*i)->OnKeyDown(e);

		}
		void KeyboardMutator::DispatchEvent(KeyPressedEventArgs& e) const {

			for (auto i = KeyboardListener::_listeners().begin(); i != KeyboardListener::_listeners().end(); ++i)
				(*i)->OnKeyPressed(e);

		}
		void KeyboardMutator::DispatchEvent(KeyUpEventArgs& e) const {

			for (auto i = KeyboardListener::_listeners().begin(); i != KeyboardListener::_listeners().end(); ++i)
				(*i)->OnKeyUp(e);

		}
		void KeyboardMutator::DispatchEvent(KeyCharEventArgs& e) const {

			for (auto i = KeyboardListener::_listeners().begin(); i != KeyboardListener::_listeners().end(); ++i)
				(*i)->OnKeyChar(e);

		}

		System::EventSource KeyboardMutator::GetEventSource() const {

			return al_get_keyboard_event_source();

		}

	}
}