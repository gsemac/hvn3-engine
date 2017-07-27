#include "MouseEventListener.h"

namespace hvn3 {

	MouseEventListener::MouseEventListener() {

		_listeners.insert(this);

	}
	MouseEventListener::~MouseEventListener() {

		// It's possible for mouse listeners to be freed after the listener collection is freed when the game state falls out of scope.
		// Make sure that we exist in the collection before trying to remove ourselves.
		if (_listeners.count(this) > 0)
			_listeners.erase(this);

	}

	void MouseEventListener::OnMouseDown(MouseDownEventArgs& e) {}
	void MouseEventListener::OnMousePressed(MousePressedEventArgs& e) {}
	void MouseEventListener::OnMouseUp(MouseUpEventArgs& e) {}
	void MouseEventListener::OnMouseMove(MouseMoveEventArgs& e) {}
	void MouseEventListener::OnMouseScroll(MouseScrollEventArgs& e) {}

	std::unordered_set<MouseEventListener*> MouseEventListener::_listeners;

}