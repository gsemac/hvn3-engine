#include "MouseEventListener.h"

namespace hvn3 {

	MouseEventListener::MouseEventListener() {

		_listeners.insert(this);

	}
	MouseEventListener::~MouseEventListener() {

		_listeners.erase(this);

	}

	void MouseEventListener::OnMouseDown(MouseDownEventArgs& e) {}
	void MouseEventListener::OnMousePressed(MousePressedEventArgs& e) {}
	void MouseEventListener::OnMouseUp(MouseUpEventArgs& e) {}
	void MouseEventListener::OnMouseMove(MouseMoveEventArgs& e) {}
	void MouseEventListener::OnMouseScroll(MouseScrollEventArgs& e) {}

	std::unordered_set<MouseEventListener*> MouseEventListener::_listeners;

}