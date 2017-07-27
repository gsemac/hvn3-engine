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
		virtual void OnKeyUp(KeyUpEventArgs& e);
		virtual void OnKeyChar(KeyCharEventArgs& e);

	protected:
		static std::unordered_set<KeyboardEventListener*>& _Listeners();

	};

}