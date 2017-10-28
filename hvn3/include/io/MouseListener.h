#pragma once
#include "io/MouseEventArgs.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class MouseController;
	}

	class MouseListener {
		friend class System::MouseController;

	public:
		MouseListener();
		virtual ~MouseListener();

		virtual void OnMouseDown(MouseDownEventArgs& e);
		virtual void OnMousePressed(MousePressedEventArgs& e);
		virtual void OnMouseUp(MouseUpEventArgs& e);
		virtual void OnMouseMove(MouseMoveEventArgs& e);
		virtual void OnMouseScroll(MouseScrollEventArgs& e);

	protected:
		static std::unordered_set<MouseListener*>& _listeners();

	};

}