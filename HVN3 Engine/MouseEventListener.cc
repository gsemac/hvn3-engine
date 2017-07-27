#include "MouseEventListener.h"

namespace hvn3 {

	MouseEventListener::MouseEventListener() {

		_Listeners().insert(this);

	}
	MouseEventListener::~MouseEventListener() {

		// When exiting the game, it's possible for the listener collection to fall out of scope and have its contents freed.
		// Attempting to access the collection at this point will throw an exception, so make sure its size is > 0.
		if(_Listeners().size() > 0)
			_Listeners().erase(this);
	
	}

	void MouseEventListener::OnMouseDown(MouseDownEventArgs& e) {}
	void MouseEventListener::OnMousePressed(MousePressedEventArgs& e) {}
	void MouseEventListener::OnMouseUp(MouseUpEventArgs& e) {}
	void MouseEventListener::OnMouseMove(MouseMoveEventArgs& e) {}
	void MouseEventListener::OnMouseScroll(MouseScrollEventArgs& e) {}

	std::unordered_set<MouseEventListener*>& MouseEventListener::_Listeners() {

		static std::unordered_set<MouseEventListener*> _listeners;

		return _listeners;

	}

}