#include "KeyboardEventListener.h"

namespace hvn3 {

	KeyboardEventListener::KeyboardEventListener() {

		// Add self to the global collection of keyboard listeners.
		_listeners.insert(this);

	}
	KeyboardEventListener::~KeyboardEventListener() {

		// Remove self from global collection of keyboard listeners.
		_listeners.erase(this);

	}

	void KeyboardEventListener::OnKeyDown(KeyDownEventArgs& e) {}
	void KeyboardEventListener::OnKeyPressed(KeyPressedEventArgs& e) {}
	void KeyboardEventListener::OnKeyReleased(KeyReleasedEventArgs& e) {}
	void KeyboardEventListener::OnKeyChar(KeyCharEventArgs& e) {}

	// Private members

	std::unordered_set<KeyboardEventListener*>KeyboardEventListener::_listeners;

}