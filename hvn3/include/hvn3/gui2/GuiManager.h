#pragma once
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/gui2/WidgetManager.h"
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/io/MouseListener.h"

namespace hvn3 {
	namespace Gui {

		class GuiManager :
			public WidgetManager,
			public KeyboardListener,
			public MouseListener
			//public System::IContextReceiver 
		{

		public:
			GuiManager();

			//void SetContext(Context context) override;
			//void OnContextChanged(ContextChangedEventArgs& e) override;

			void OnDraw(DrawEventArgs& e) override;
			void OnUpdate(UpdateEventArgs& e) override;

			void OnKeyDown(KeyDownEventArgs& e) override;
			void OnKeyPressed(KeyPressedEventArgs& e) override;
			void OnKeyUp(KeyUpEventArgs& e) override;
			void OnKeyChar(KeyCharEventArgs& e) override;
			void OnKeyboardLost(KeyboardLostEventArgs& e) override;
			void OnKeyboardFound(KeyboardFoundEventArgs& e) override;

			void OnMouseDown(MouseDownEventArgs& e) override;
			void OnMousePressed(MousePressedEventArgs& e) override;
			void OnMouseReleased(MouseReleasedEventArgs& e) override;
			void OnMouseMove(MouseMoveEventArgs& e) override;
			void OnMouseScroll(MouseScrollEventArgs& e) override;

		};

	}
}