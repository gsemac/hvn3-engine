#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/io/MouseListener.h"
#include <memory>
#include <list>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class IWidgetRenderer;

		class WidgetManager : public IDrawable, public IUpdatable, public KeyboardListener, public MouseListener {

			typedef std::shared_ptr<IWidgetRenderer> renderer_ptr_type;
			typedef std::list<std::unique_ptr<IWidget>> widget_collection_type;

		public:
			WidgetManager();
			WidgetManager(renderer_ptr_type& renderer);
			
			void Add(IWidget* widget);
			void Add(std::unique_ptr<IWidget>& widget);

			void SetRenderer(renderer_ptr_type& renderer);
			const IWidgetRenderer& Renderer();
			renderer_ptr_type& GetRenderer();

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
			renderer_ptr_type _renderer;
			widget_collection_type _widgets;

			IWidget* _widget_hovered;
			PointF _last_mouse_position;

			void _initialize();

			// Returns a pointer to the widget renderer assigned to this manager. 
			// IF no renderer has been assigned, assigns and returns the default renderer.
			renderer_ptr_type& _getRenderer();
			
		};

	}
}