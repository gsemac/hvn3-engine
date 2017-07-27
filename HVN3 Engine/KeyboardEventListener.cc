#include "KeyboardEventListener.h"

namespace hvn3 {

	KeyboardEventListener::KeyboardEventListener() {

		// Add self to the global collection of keyboard listeners.
		_Listeners().insert(this);

	}
	KeyboardEventListener::~KeyboardEventListener() {

		// When exiting the game, it's possible for the listener collection to fall out of scope and have its contents freed.
		// Attempting to access the collection at this point will throw an exception, so make sure its size is > 0.
		if (_Listeners().size() > 0)
			_Listeners().erase(this);

	}

	void KeyboardEventListener::OnKeyDown(KeyDownEventArgs& e) {}
	void KeyboardEventListener::OnKeyPressed(KeyPressedEventArgs& e) {}
	void KeyboardEventListener::OnKeyUp(KeyUpEventArgs& e) {}
	void KeyboardEventListener::OnKeyChar(KeyCharEventArgs& e) {}

	std::unordered_set<KeyboardEventListener*>& KeyboardEventListener::_Listeners() {

		static std::unordered_set<KeyboardEventListener*> _listeners;

		return _listeners;

	}

}