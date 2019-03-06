#include "hvn3/io/Display.h"
#include "hvn3/gui2/GuiManager.h"

namespace hvn3 {
	namespace Gui {

		GuiManager::GuiManager() {

			_using_mouse_transform = false;

		}
		void GuiManager::OnDraw(DrawEventArgs& e) {
			WidgetManager::OnDraw(e);
		}
		void GuiManager::OnUpdate(UpdateEventArgs& e) {
			WidgetManager::OnUpdate(e);
		}
		void GuiManager::OnKeyDown(KeyDownEventArgs& e) {
			WidgetManager::OnKeyDown(e);
		}
		void GuiManager::OnKeyPressed(KeyPressedEventArgs& e) {
			WidgetManager::OnKeyPressed(e);
		}
		void GuiManager::OnKeyUp(KeyUpEventArgs& e) {
			WidgetManager::OnKeyUp(e);
		}
		void GuiManager::OnKeyChar(KeyCharEventArgs& e) {
			WidgetManager::OnKeyChar(e);
		}
		void GuiManager::OnKeyboardLost(KeyboardLostEventArgs& e) {}
		void GuiManager::OnKeyboardFound(KeyboardFoundEventArgs& e) {}
		void GuiManager::OnMouseDown(MouseDownEventArgs& e) {

			if (_using_mouse_transform) {

				MouseDownEventArgs args(e.Button(), e.DisplayPosition(), _mouse_transform.TransformPoint(e.DisplayPosition()));

				WidgetManager::OnMouseDown(args);

			}
			else
				WidgetManager::OnMouseDown(e);

		}
		void GuiManager::OnMousePressed(MousePressedEventArgs& e) {

			if (_using_mouse_transform) {

				MousePressedEventArgs args(e.Button(), e.DisplayPosition(), _mouse_transform.TransformPoint(e.DisplayPosition()), e.Clicks());

				WidgetManager::OnMousePressed(args);

			}
			else
				WidgetManager::OnMousePressed(e);

		}
		void GuiManager::OnMouseReleased(MouseReleasedEventArgs& e) {

			if (_using_mouse_transform) {

				MouseReleasedEventArgs args(e.Button(), e.DisplayPosition(), _mouse_transform.TransformPoint(e.DisplayPosition()));

				WidgetManager::OnMouseReleased(args);

			}
			else
				WidgetManager::OnMouseReleased(e);


		}
		void GuiManager::OnMouseMove(MouseMoveEventArgs& e) {

			if (_using_mouse_transform) {

				MouseMoveEventArgs args(e.DisplayPosition(), _mouse_transform.TransformPoint(e.DisplayPosition()));

				WidgetManager::OnMouseMove(args);

			}
			else
				WidgetManager::OnMouseMove(e);

		}
		void GuiManager::OnMouseScroll(MouseScrollEventArgs& e) {
			WidgetManager::OnMouseScroll(e);
		}
		void GuiManager::SetMouseTransform(const Graphics::Transform& transform) {

			_mouse_transform = transform;
			_using_mouse_transform = true;

		}

	}
}