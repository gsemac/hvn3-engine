#include "io/KeyboardListener.h"

namespace hvn3 {

	KeyboardListener::KeyboardListener() {

		// Add self to the global collection of keyboard listeners.
		_listeners().insert(this);

	}
	KeyboardListener::~KeyboardListener() {

		// Be careful! We can't have listeners popping themselves off the stack after the static collection has been deinitialized.
		// The game manager takes care of this by freeing all objects (and thus any listener instances) before being deinitialized itself.
		_listeners().erase(this);

	}

	void KeyboardListener::OnKeyDown(KeyDownEventArgs& e) {}
	void KeyboardListener::OnKeyPressed(KeyPressedEventArgs& e) {}
	void KeyboardListener::OnKeyUp(KeyUpEventArgs& e) {}
	void KeyboardListener::OnKeyChar(KeyCharEventArgs& e) {}

	std::unordered_set<KeyboardListener*>& KeyboardListener::_listeners() {

		static std::unordered_set<KeyboardListener*> _listeners;

		return _listeners;

	}

}