#include "hvn3/io/MouseListenerBase.h"

namespace hvn3 {

	// Public methods

	void MouseListenerBase::OnEvent(MouseDownEventArgs& e) {
		OnMouseDown(e);
	}
	void MouseListenerBase::OnEvent(MousePressedEventArgs& e) {
		OnMousePressed(e);
	}
	void MouseListenerBase::OnEvent(MouseReleasedEventArgs& e) {
		OnMouseReleased(e);
	}
	void MouseListenerBase::OnEvent(MouseMoveEventArgs& e) {
		OnMouseMove(e);
	}
	void MouseListenerBase::OnEvent(MouseScrollEventArgs& e) {
		OnMouseScroll(e);
	}

	void MouseListenerBase::OnMouseDown(MouseDownEventArgs& e) {}
	void MouseListenerBase::OnMousePressed(MousePressedEventArgs& e) {}
	void MouseListenerBase::OnMouseReleased(MouseReleasedEventArgs& e) {}
	void MouseListenerBase::OnMouseMove(MouseMoveEventArgs& e) {}
	void MouseListenerBase::OnMouseScroll(MouseScrollEventArgs& e) {}

}