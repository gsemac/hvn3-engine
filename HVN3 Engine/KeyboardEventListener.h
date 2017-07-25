#pragma once
#include "KeyEventArgs.h"
#include <unordered_set>
//#include <queue>

namespace hvn3 {

	namespace System {
		class KeyboardController;
	}

	class KeyboardEventListener {
		friend class System::KeyboardController;

	public:
		KeyboardEventListener();
		virtual ~KeyboardEventListener();

		virtual void OnKeyDown(KeyDownEventArgs& e);
		virtual void OnKeyPressed(KeyPressedEventArgs& e);
		virtual void OnKeyReleased(KeyReleasedEventArgs& e);
		virtual void OnKeyChar(KeyCharEventArgs& e);

	private:
		static std::unordered_set<KeyboardEventListener*> _listeners;

	};

}