#include "KeyboardEventListener.h"

namespace hvn3 {

	KeyboardEventListener::KeyboardEventListener() {

		// Add self to the global collection of keyboard listeners.
		_listeners.insert(this);

	}
	KeyboardEventListener::~KeyboardEventListener() {

		// It's possible for mouse listeners to be freed after the listener collection is freed when the game state falls out of scope.
		// Make sure that we exist in the collection before trying to remove ourselves.
		if (_listeners.count(this) > 0)
			_listeners.erase(this);

	}

	void KeyboardEventListener::OnKeyDown(KeyDownEventArgs& e) {}
	void KeyboardEventListener::OnKeyPressed(KeyPressedEventArgs& e) {}
	void KeyboardEventListener::OnKeyUp(KeyUpEventArgs& e) {}
	void KeyboardEventListener::OnKeyChar(KeyCharEventArgs& e) {}

	// Private members

	std::unordered_set<KeyboardEventListener*>KeyboardEventListener::_listeners;

}