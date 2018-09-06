#pragma once
#include "hvn3/io/IKeyboardListener.h"
#include "hvn3/io/ListenerBase.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class KeyboardMutator;
	}

	class KeyboardListener :
		public ListenerBase<IKeyboardListener> {

	public:
		void OnKeyDown(KeyDownEventArgs& e) override;
		void OnKeyPressed(KeyPressedEventArgs& e) override;
		void OnKeyUp(KeyUpEventArgs& e) override;
		void OnKeyChar(KeyCharEventArgs& e) override;
		void OnKeyboardLost(KeyboardLostEventArgs& e) override;
		void OnKeyboardFound(KeyboardFoundEventArgs& e) override;

	};

}