#pragma once

#include "hvn3/io/IODefs.h"

#include <array>
#include <cstddef>
#include <functional>

#include <allegro5/keyboard.h>

namespace hvn3 {

	class KeyboardState {

		struct KeyState {

			bool held; // whether or not key is held
			bool pressed; // whether or not key is pressed (instantaneous)
			bool locked; // if locked, pressed is false until released and pressed again (for single key presses)
			bool released; // whether or not the key was released (instaneous)

			KeyState();

		};

	public:
		class Iterator {

		public:
			class KeyStateWrapper {

			public:
				KeyStateWrapper() = default;
				KeyStateWrapper(KeyState state, Key key);

				bool Held() const;
				bool Pressed() const;
				bool Released() const;
				Key Key() const;

			private:
				hvn3::Key _key;
				KeyState _state;

			};

			Iterator(KeyboardState* state, size_t keyIndex);

			bool operator==(const Iterator& other);
			bool operator!=(const Iterator& other);
			Iterator& operator++();
			Iterator operator++(int);
			KeyStateWrapper& operator*();
			KeyStateWrapper* operator->();
			const KeyStateWrapper& operator*() const;
			const KeyStateWrapper* operator->() const;

		private:
			void _initKeyState() const;

			KeyboardState* _state;
			size_t _key_index;
			mutable KeyStateWrapper _key_state;

		};

		friend class Iterator;

		KeyboardState();

		bool KeyPressed(int keyCode);
		bool KeyDown(int keyCode);
		bool KeyReleased(int keyCode);
		bool KeyPressedOrHeld(int keyCode);
		bool KeyPressed(Key key);
		bool KeyDown(Key key);
		bool KeyReleased(Key key);
		bool KeyPressedOrHeld(Key key);

		int LastChar();
		int LastKey();
		KeyModifiers Modifiers();
		void ClearLastChar();
		void ClearLastKey();

		void ClearKeyStates(bool pressed, bool released, bool held);
		void ClearKeyStates();
		void SetKeyState(int keyCode, bool pressed);
		void SetLastChar(int keyChar);

		Iterator begin();
		Iterator end();

	private:
		std::array<KeyState, ALLEGRO_KEY_MAX> _key_states;
		bool _pressed_any;
		bool _released_any;
		int _held_count;
		int _last_char;
		int _last_key;
		int _modifier;

	};

}