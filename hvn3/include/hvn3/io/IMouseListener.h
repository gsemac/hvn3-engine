#pragma once

namespace hvn3 {

	class MouseDownEventArgs;
	class MouseMoveEventArgs;
	class MousePressedEventArgs;
	class MouseReleasedEventArgs;
	class MouseScrollEventArgs;

	class IMouseListener {

	public:
		virtual void OnMouseDown(MouseDownEventArgs& e) = 0;
		virtual void OnMousePressed(MousePressedEventArgs& e) = 0;
		virtual void OnMouseReleased(MouseReleasedEventArgs& e) = 0;
		virtual void OnMouseMove(MouseMoveEventArgs& e) = 0;
		virtual void OnMouseScroll(MouseScrollEventArgs& e) = 0;

	};

}