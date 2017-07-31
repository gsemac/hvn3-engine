#include "KeyboardListener.h"

namespace hvn3 {

	KeyboardListener::KeyboardListener() {

		// Add self to the global collection of keyboard listeners.
		_listeners().insert(this);

	}
	KeyboardListener::~KeyboardListener() {

		// When exiting the game, it's possible for the listener collection to fall out of scope and have its contents freed.
		// Attempting to access the collection at this point will throw an exception, so make sure its size is > 0.
		if (_listeners().size() > 0)
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