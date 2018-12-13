#pragma once
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/gui2/WidgetManager.h"
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/io/MouseListener.h"
#include <vector>

namespace hvn3 {
	namespace Gui {

		class GuiManager :
			public IManager,
			public WidgetManager,
			public KeyboardListener,
			public MouseListener {

		public:
			GuiManager();

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

			void SetMouseTransform(const Graphics::Transform& transform);

		private:
			Graphics::Transform _mouse_transform;
			bool _using_mouse_transform;

		};

	}

	template<>
	struct ManagerIdTraits<GUI_MANAGER> {
		typedef Gui::GuiManager type;
	};

}