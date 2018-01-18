#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetManager.h"

namespace hvn3 {
	namespace Gui {

		WidgetManager::WidgetManager() {
		}
		WidgetManager::WidgetManager(renderer_ptr_type& renderer) :
			_renderer(std::move(renderer)) {
		}

		void WidgetManager::Add(IWidget* widget) {

			Add(std::unique_ptr<IWidget>(widget));

		}
		void WidgetManager::Add(std::unique_ptr<IWidget>& widget) {

			widget->SetManager(this);

			_widgets.emplace_back(std::move(widget));

		}

		void WidgetManager::SetRenderer(renderer_ptr_type& renderer) {

			_renderer = renderer;

		}
		const IWidgetRenderer& WidgetManager::Renderer() {

			return *_getRenderer();

		}
		WidgetManager::renderer_ptr_type& WidgetManager::GetRenderer() {

			return _renderer;

		}

		void WidgetManager::OnDraw(DrawEventArgs& e) {

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
				_getRenderer()->DrawWidget(e.Graphics(), *(*i));

		}
		void WidgetManager::OnUpdate(UpdateEventArgs& e) {



		}

		void WidgetManager::OnKeyDown(KeyDownEventArgs& e) {}
		void WidgetManager::OnKeyPressed(KeyPressedEventArgs& e) {}
		void WidgetManager::OnKeyUp(KeyUpEventArgs& e) {}
		void WidgetManager::OnKeyChar(KeyCharEventArgs& e) {}

		void WidgetManager::OnMouseDown(MouseDownEventArgs& e) {}
		void WidgetManager::OnMousePressed(MousePressedEventArgs& e) {}
		void WidgetManager::OnMouseReleased(MouseReleasedEventArgs& e) {}
		void WidgetManager::OnMouseMove(MouseMoveEventArgs& e) {}
		void WidgetManager::OnMouseScroll(MouseScrollEventArgs& e) {}



		WidgetManager::renderer_ptr_type& WidgetManager::_getRenderer() {

			if (!_renderer)
				_renderer = renderer_ptr_type(new DefaultWidgetRenderer);

			return _renderer;

		}

	}
}