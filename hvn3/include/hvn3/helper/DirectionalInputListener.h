#pragma once
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/utility/Direction8.h"

namespace hvn3 {

	class DirectionalInputListener : 
		public KeyboardListener {

	public:
		DirectionalInputListener(int directions);

		void OnKeyPressed(KeyPressedEventArgs& e) override;
		void OnKeyUp(KeyUpEventArgs& e) override;
		void OnKeyboardLost(KeyboardLostEventArgs& e) override;

		// Sets the four directional keys to listen for.
		void SetKeys(Key up, Key left, Key down, Key right);
		// Returns the current direction based on the directional keys currently pressed.
		Direction8 Direction() const;
		// Returns the direction based on the directional keys that were last pressed.
		Direction8 LastDirection() const;

	protected:
		enum KEYDIR {
			KEYDIR_UP,
			KEYDIR_DOWN,
			KEYDIR_LEFT,
			KEYDIR_RIGHT
		};
		struct KeyData {
			Key key;
			bool pressed;
		};

		KeyData GetKeyData(KEYDIR keydir);
		int DirectionCount() const;

	private:
		void _resetKeyStates();
		void _updateLastDirection();

		int _directions;
		DIRECTION _last_hdir;
		DIRECTION _last_vdir;
		Direction8 _last_dir;
		KeyData _keys[4];

	};

}