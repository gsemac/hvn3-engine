#pragma once
#include "MouseEventArgs.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class MouseController;
	}

	class MouseEventListener {
		friend class System::MouseController;

	public:
		MouseEventListener();
		virtual ~MouseEventListener();

		virtual void OnMouseDown(MouseDownEventArgs& e);
		virtual void OnMousePressed(MousePressedEventArgs& e);
		virtual void OnMouseUp(MouseUpEventArgs& e);
		virtual void OnMouseMove(MouseMoveEventArgs& e);
		virtual void OnMouseScroll(MouseScrollEventArgs& e);

	protected:
		static std::unordered_set<MouseEventListener*>& _Listeners();

	};

}