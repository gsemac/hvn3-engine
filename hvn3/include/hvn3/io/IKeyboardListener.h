#pragma once

namespace hvn3 {

	class KeyboardLostEventArgs;
	class KeyboardFoundEventArgs;
	class KeyCharEventArgs;
	class KeyDownEventArgs;
	class KeyPressedEventArgs;
	class KeyUpEventArgs;

	class IKeyboardListener {

	public:
		virtual void OnKeyDown(KeyDownEventArgs& e) = 0;
		virtual void OnKeyPressed(KeyPressedEventArgs& e) = 0;
		virtual void OnKeyUp(KeyUpEventArgs& e) = 0;
		virtual void OnKeyChar(KeyCharEventArgs& e) = 0;
		virtual void OnKeyboardLost(KeyboardLostEventArgs& e) = 0;
		virtual void OnKeyboardFound(KeyboardFoundEventArgs& e) = 0;

	};

}