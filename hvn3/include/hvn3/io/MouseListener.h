#pragma once
#include "hvn3/io/IMouseListener.h"
#include "hvn3/io/ListenerBase.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class MouseMutator;
	}

	class MouseListener :
		public ListenerBase<IMouseListener> {

	public:
		void OnMouseDown(MouseDownEventArgs& e) override;
		void OnMousePressed(MousePressedEventArgs& e) override;
		void OnMouseReleased(MouseReleasedEventArgs& e) override;
		void OnMouseMove(MouseMoveEventArgs& e) override;
		void OnMouseScroll(MouseScrollEventArgs& e) override;

	};

}