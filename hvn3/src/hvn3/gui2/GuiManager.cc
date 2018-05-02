#include "hvn3/gui2/GuiManager.h"

namespace hvn3 {
	namespace Gui {

		GuiManager::GuiManager() {}
		void GuiManager::AddChildWidgetManager(WidgetManager* manger) {

		}
		void GuiManager::RemoveChildWidgetManager(WidgetManager* manager) {

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
		void GuiManager::OnMouseDown(MouseDownEventArgs& e) {
			WidgetManager::OnMouseDown(e);
		}
		void GuiManager::OnMousePressed(MousePressedEventArgs& e) {
			WidgetManager::OnMousePressed(e);
		}
		void GuiManager::OnMouseReleased(MouseReleasedEventArgs& e) {
			WidgetManager::OnMouseReleased(e);
		}
		void GuiManager::OnMouseMove(MouseMoveEventArgs& e) {
			WidgetManager::OnMouseMove(e);
		}
		void GuiManager::OnMouseScroll(MouseScrollEventArgs& e) {
			WidgetManager::OnMouseScroll(e);
		}

	}
}