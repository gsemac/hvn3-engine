#pragma once
#include "hvn3/io/KeyboardEventArgs.h"

namespace hvn3 {

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