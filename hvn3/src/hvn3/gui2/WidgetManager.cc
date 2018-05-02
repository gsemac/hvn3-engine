#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetManager.h"
#define HVN3_DEFAULT_WIDGET_Z 1

namespace hvn3 {
	namespace Gui {

		WidgetManager::WidgetData::WidgetData(std::unique_ptr<IWidget>& widget) :
			widget(std::move(widget)) {
			z = HVN3_DEFAULT_WIDGET_Z;
		}
		IWidget& WidgetManager::WidgetData::GetRef() {
			return *widget;
		}


		WidgetManager::WidgetManager() :
			_dockable_region(0.0f, 0.0f) {

			_initialize();

		}
		WidgetManager::WidgetManager(renderer_ptr_type& renderer) :
			_renderer(std::move(renderer)),
			_dockable_region(0.0f, 0.0f) {

			_initialize();

		}

		void WidgetManager::Add(IWidget* widget) {

			Add(std::unique_ptr<IWidget>(widget));

		}
		void WidgetManager::Add(std::unique_ptr<IWidget>& widget) {

			widget->SetManager(this);

			_widgets.emplace_back(WidgetData(widget));

		}
		void WidgetManager::BringToFront(IWidget* widget) {
			auto iter = _findWidget(widget);
			if (iter == _widgets.end())
				return;
			iter->z = 0;
			_resort_required = true;
		}
		void WidgetManager::SendToBack(IWidget* widget) {
			auto iter = _findWidget(widget);
			if (iter == _widgets.end())
				return;
			iter->z = _widgets.front().z + 1;
			_resort_required = true;
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
		const RectangleF& WidgetManager::DockableRegion() const {
			return _dockable_region;
		}
		void WidgetManager::SetDockableRegion(const RectangleF& value) {
			_dockable_region = value;
		}

		void WidgetManager::OnDraw(DrawEventArgs& e) {

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
				_getRenderer()->DrawWidget(e.Graphics(), i->GetRef(), i->rendererArgs);

		}
		void WidgetManager::OnUpdate(UpdateEventArgs& e) {

			if (_widget_hovered != nullptr)
				_widget_hovered->HandleEvent(WidgetMouseHoverEventArgs(_widget_hovered, _last_mouse_position, e.Delta()));

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i) {

				// If the widget is docked, adjust its size and position accordingly.
				_applyDockStyle(i->widget.get());

				// Call the update event for the widget.
				i->widget->HandleEvent(WidgetUpdateEventArgs(i->widget.get(), e.Delta()));

				// Update the widget's transition data (i.e. update animations).
				i->rendererArgs.UpdateTransitionData(static_cast<float>(e.Delta()));

			}

			// If sorting is required, sort the list of widgets, and then reset their z values.
			if (_resort_required) {
				_widgets.sort([](const WidgetData& lhs, const WidgetData& rhs) {
					return lhs.z > rhs.z;
				});
				// Reset the z values after sorting so that BringToFront/SendToBack continue to work.
				for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
					i->z = HVN3_DEFAULT_WIDGET_Z;
				_resort_required = false;
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

			IWidget* widget_hovered = nullptr;

			// Find the widget that the mouse is currently hovering over.
			for (auto i = _widgets.rbegin(); i != _widgets.rend(); ++i) {
				IWidget* widget = i->widget.get();
				widget->HandleEvent(WidgetMouseMoveEventArgs(widget, WidgetEventType::OnMouseMove, e));
				if (widget_hovered == nullptr && Math::Geometry::PointIn(e.Position(), RectangleF(widget->Position(), widget->Size())))
					widget_hovered = widget;
			}

			// Dispatch the appropriate events if the hovered widget has changed.
			if (widget_hovered != _widget_hovered) {
				if (_widget_hovered != nullptr)
					_widget_hovered->HandleEvent(WidgetMouseMoveEventArgs(_widget_hovered, WidgetEventType::OnMouseLeave, e));
				if (widget_hovered != nullptr)
					widget_hovered->HandleEvent(WidgetMouseMoveEventArgs(widget_hovered, WidgetEventType::OnMouseEnter, e));
			}

			// Update the hovered widget.
			_last_mouse_position = e.Position();
			_widget_hovered = widget_hovered;

		}
		void WidgetManager::OnMouseScroll(MouseScrollEventArgs& e) {}



		void WidgetManager::_initialize() {
			_widget_hovered = nullptr;
			_widget_held = nullptr;
			_resort_required = false;
		}
		WidgetManager::widget_collection_type::iterator WidgetManager::_findWidget(IWidget* widget) {
			return std::find_if(_widgets.begin(), _widgets.end(), [=](const WidgetData& x) { return x.widget.get() == widget; });
		}
		void WidgetManager::_applyDockStyle(IWidget* widget) {

			switch (widget->DockStyle()) {

			case DockStyle::Top:
				widget->SetPosition(0.0f, 0.0f);
				widget->SetWidth(DockableRegion().Width());
				break;

			case DockStyle::Bottom:
				widget->SetPosition(0.0f, DockableRegion().Height() - widget->Height());
				widget->SetWidth(DockableRegion().Width());
				break;

			case DockStyle::Left:
				widget->SetPosition(0.0f, 0.0f);
				widget->SetHeight(DockableRegion().Height());
				break;

			case DockStyle::Right:
				widget->SetPosition(DockableRegion().Width() - widget->Width(), 0.0f);
				widget->SetHeight(DockableRegion().Height());
				break;

			case DockStyle::Fill:
				widget->SetPosition(0.0f, 0.0f);
				widget->SetSize(DockableRegion().Size());
				break;

			default:
				break;

			}

		}
		WidgetManager::renderer_ptr_type& WidgetManager::_getRenderer() {

			if (!_renderer)
				_renderer = renderer_ptr_type(new DefaultWidgetRenderer);

			return _renderer;

		}

	}
}