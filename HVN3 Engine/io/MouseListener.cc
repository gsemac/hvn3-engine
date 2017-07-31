#include "MouseListener.h"

namespace hvn3 {

	MouseListener::MouseListener() {

		_listeners().insert(this);

	}
	MouseListener::~MouseListener() {

		// When exiting the game, it's possible for the listener collection to fall out of scope and have its contents freed.
		// Attempting to access the collection at this point will throw an exception, so make sure its size is > 0.
		if(_listeners().size() > 0)
			_listeners().erase(this);
	
	}

	void MouseListener::OnMouseDown(MouseDownEventArgs& e) {}
	void MouseListener::OnMousePressed(MousePressedEventArgs& e) {}
	void MouseListener::OnMouseUp(MouseUpEventArgs& e) {}
	void MouseListener::OnMouseMove(MouseMoveEventArgs& e) {}
	void MouseListener::OnMouseScroll(MouseScrollEventArgs& e) {}

	std::unordered_set<MouseListener*>& MouseListener::_listeners() {

		static std::unordered_set<MouseListener*> _listeners;

		return _listeners;

	}

}