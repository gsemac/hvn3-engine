#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetManager.h"

namespace hvn3 {
	namespace Gui {

		WidgetManager::WidgetCollectionItem::WidgetCollectionItem(std::unique_ptr<IWidget>& widget) :
			widget(std::move(widget)) {
		}
		IWidget& WidgetManager::WidgetCollectionItem::GetRef() {
			return *widget;
		}


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

			_widgets.emplace_back(WidgetCollectionItem(widget));

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
				_getRenderer()->DrawWidget(e.Graphics(), i->GetRef(), i->rendererArgs);

		}
		void WidgetManager::OnUpdate(UpdateEventArgs& e) {

			if (_widget_hovered != nullptr)
				_widget_hovered->HandleEvent(WidgetMouseHoverEventArgs(_widget_hovered, _last_mouse_position, e.Delta()));

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i) {
				i->widget->HandleEvent(WidgetUpdateEventArgs(i->widget.get(), e.Delta()));
				i->rendererArgs.UpdateTransitionData(e.Delta());
			}

		}

		void WidgetManager::OnKeyDown(KeyDownEventArgs& e) {}
		void WidgetManager::OnKeyPressed(KeyPressedEventArgs& e) {}
		void WidgetManager::OnKeyUp(KeyUpEventArgs& e) {}
		void WidgetManager::OnKeyChar(KeyCharEventArgs& e) {}

		void WidgetManager::OnMouseDown(MouseDownEventArgs& e) {
			// Call the mouse-down event for the currently-hovered widget.
			if (_widget_hovered != nullptr && _widget_held == nullptr) {
				_widget_hovered->HandleEvent(WidgetMouseEventArgs(_widget_hovered, WidgetEventType::OnMouseDown, e));
				_widget_held = _widget_hovered;
			}
		}
		void WidgetManager::OnMousePressed(MousePressedEventArgs& e) {}
		void WidgetManager::OnMouseReleased(MouseReleasedEventArgs& e) {
			// Only call the mouse-up event for a widget we've previously called the mouse-down event for.
			if (_widget_held != nullptr) {
				_widget_held->HandleEvent(WidgetMouseEventArgs(_widget_held, WidgetEventType::OnMouseUp, e));
				// If the mouse was released on the same widget that it went down on, consider it a click.
				if (_widget_hovered == _widget_held)
					_widget_held->HandleEvent(WidgetMouseEventArgs(_widget_held, WidgetEventType::OnMouseClick, e));
				_widget_held = nullptr;
			}
		}
		void WidgetManager::OnMouseMove(MouseMoveEventArgs& e) {

			// Find the widget that the mouse is currently hovering over, if any.
			// Assume the widgets are ordered such that topmost widgets are first.

			IWidget* widget_hovered = nullptr;

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i) {
				IWidget* widget = i->widget.get();
				if (Math::Geometry::PointIn(e.Position(), RectangleF(widget->Position(), widget->Size()))) {
					widget_hovered = widget;
					break;
				}
			}

			if (widget_hovered != _widget_hovered) {
				if (_widget_hovered != nullptr)
					_widget_hovered->HandleEvent(WidgetMouseMoveEventArgs(_widget_hovered, WidgetEventType::OnMouseLeave, e));
				if (widget_hovered != nullptr)
					widget_hovered->HandleEvent(WidgetMouseMoveEventArgs(widget_hovered, WidgetEventType::OnMouseEnter, e));
			}

			_last_mouse_position = e.Position();
			_widget_hovered = widget_hovered;

		}
		void WidgetManager::OnMouseScroll(MouseScrollEventArgs& e) {}



		void WidgetManager::_initialize() {
			_widget_hovered = nullptr;
			_widget_held = nullptr;
		}
		WidgetManager::renderer_ptr_type& WidgetManager::_getRenderer() {

			if (!_renderer)
				_renderer = renderer_ptr_type(new DefaultWidgetRenderer);

			return _renderer;

		}

	}
}