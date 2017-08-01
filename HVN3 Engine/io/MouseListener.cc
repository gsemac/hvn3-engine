#include "MouseListener.h"

namespace hvn3 {

	MouseListener::MouseListener() {

		_listeners().insert(this);

	}
	MouseListener::~MouseListener() {

		// Be careful! We can't have listeners popping themselves off the stack after the static collection has been deinitialized.
		// The game manager takes care of this by freeing all objects (and thus any listener instances) before being deinitialized itself.
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