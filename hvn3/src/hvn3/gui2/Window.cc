#include "hvn3/gui2/Window.h"

namespace hvn3 {
	namespace Gui {

		Window::Window(float x, float y, float width, float height, const String& text) :
			WidgetBase(x, y, width, height) {
			_initializeMembers();
			SetText(text);
		}

		void Window::OnMouseDown(WidgetMouseEventArgs& e) {
			
			WidgetBase::OnMouseDown(e);
			
			_mouse_down_position = (e.Position() - Position());
			_is_mouse_dragging = true;
			
			if (GetManager() != nullptr)
				GetManager()->BringToFront(this);

		}
		void Window::OnMouseUp(WidgetMouseEventArgs& e) {
			WidgetBase::OnMouseUp(e);
			_is_mouse_dragging = false;
		}
		void Window::OnMouseMove(WidgetMouseMoveEventArgs& e) {
			WidgetBase::OnMouseMove(e);
			if (_is_mouse_dragging)
				SetPosition(e.Position() - _mouse_down_position);
		}



		void Window::_initializeMembers() {
			SetIdentifier("window");
		}

	}
}