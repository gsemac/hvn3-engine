#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetManager.h"

namespace hvn3 {
	namespace Gui {

		WidgetManager::WidgetManager() {

			_initialize();

		}
		WidgetManager::WidgetManager(renderer_ptr_type& renderer) :
			_renderer(std::move(renderer)) {

			_initialize();

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
			
			if (_widget_hovered != nullptr)
				_widget_hovered->OnMouseHover(WidgetMouseHoverEventArgs(_widget_hovered, _last_mouse_position));

		}

		void WidgetManager::OnKeyDown(KeyDownEventArgs& e) {}
		void WidgetManager::OnKeyPressed(KeyPressedEventArgs& e) {}
		void WidgetManager::OnKeyUp(KeyUpEventArgs& e) {}
		void WidgetManager::OnKeyChar(KeyCharEventArgs& e) {}

		void WidgetManager::OnMouseDown(MouseDownEventArgs& e) {}
		void WidgetManager::OnMousePressed(MousePressedEventArgs& e) {}
		void WidgetManager::OnMouseReleased(MouseReleasedEventArgs& e) {}
		void WidgetManager::OnMouseMove(MouseMoveEventArgs& e) {

			// Find the widget that the mouse is currently hovering over, if any.
			// Assume the widgets are ordered such that topmost widgets are first.

			IWidget* widget_hovered = nullptr;

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i) {

				IWidget* widget = i->get();

				if (Math::Geometry::PointIn(e.Position(), RectangleF(widget->Position(), widget->Size()))) {
					widget_hovered = widget;
					break;
				}

			}

			_last_mouse_position = e.Position();
			_widget_hovered = widget_hovered;

		}
		void WidgetManager::OnMouseScroll(MouseScrollEventArgs& e) {}


		void WidgetManager::_initialize() {

			_widget_hovered = nullptr;

		}
		WidgetManager::renderer_ptr_type& WidgetManager::_getRenderer() {

			if (!_renderer)
				_renderer = renderer_ptr_type(new DefaultWidgetRenderer);

			return _renderer;

		}

	}
}