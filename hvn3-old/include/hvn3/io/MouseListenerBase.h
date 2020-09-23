#pragma once

#include "hvn3/events/EventListenerBase.h"
#include "hvn3/io/IMouseListener.h"

namespace hvn3 {

	class MouseListenerBase :
		public IMouseListener,
		public EventListenerBase<
		IMouseListener::MouseDownEventArgs,
		IMouseListener::MousePressedEventArgs,
		IMouseListener::MouseReleasedEventArgs,
		IMouseListener::MouseMoveEventArgs,
		IMouseListener::MouseScrollEventArgs
		> {

	public:
		void OnEvent(MouseDownEventArgs& e) override;
		void OnEvent(MousePressedEventArgs& e) override;
		void OnEvent(MouseReleasedEventArgs& e) override;
		void OnEvent(MouseMoveEventArgs& e) override;
		void OnEvent(MouseScrollEventArgs& e) override;

		void OnMouseDown(MouseDownEventArgs& e) override;
		void OnMousePressed(MousePressedEventArgs& e) override;
		void OnMouseReleased(MouseReleasedEventArgs& e) override;
		void OnMouseMove(MouseMoveEventArgs& e) override;
		void OnMouseScroll(MouseScrollEventArgs& e) override;

	};

}