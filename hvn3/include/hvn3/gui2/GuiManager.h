#pragma once
#include "hvn3/gui2/WidgetManager.h"
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/io/MouseListener.h"
#include <list>

namespace hvn3 {
	namespace Gui {

		class GuiManager :
			public WidgetManager,
			public KeyboardListener,
			public MouseListener {

		public:
			GuiManager();

			void AddChildWidgetManager(WidgetManager* manger);
			void RemoveChildWidgetManager(WidgetManager* manager);

			void OnDraw(DrawEventArgs& e) override;
			void OnUpdate(UpdateEventArgs& e) override;

			void OnKeyDown(KeyDownEventArgs& e) override;
			void OnKeyPressed(KeyPressedEventArgs& e) override;
			void OnKeyUp(KeyUpEventArgs& e) override;
			void OnKeyChar(KeyCharEventArgs& e) override;

			void OnMouseDown(MouseDownEventArgs& e) override;
			void OnMousePressed(MousePressedEventArgs& e) override;
			void OnMouseReleased(MouseReleasedEventArgs& e) override;
			void OnMouseMove(MouseMoveEventArgs& e) override;
			void OnMouseScroll(MouseScrollEventArgs& e) override;

		private:
			std::list<WidgetManager*> _child_managers;

		};

	}
}