#pragma once
#include "io/KeyboardEventArgs.h"
#include <unordered_set>
//#include <queue>

namespace hvn3 {

	namespace System {
		class KeyboardController;
	}

	class KeyboardListener {
		friend class System::KeyboardController;

	public:
		KeyboardListener();
		virtual ~KeyboardListener();

		virtual void OnKeyDown(KeyDownEventArgs& e);
		virtual void OnKeyPressed(KeyPressedEventArgs& e);
		virtual void OnKeyUp(KeyUpEventArgs& e);
		virtual void OnKeyChar(KeyCharEventArgs& e);

	protected:
		static std::unordered_set<KeyboardListener*>& _listeners();

	};

}