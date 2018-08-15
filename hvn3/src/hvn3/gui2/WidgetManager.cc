#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetManager.h"
#include <algorithm>
#include <cassert>
#define HVN3_DEFAULT_WIDGET_Z 1

namespace hvn3 {
	namespace Gui {

		WidgetManager::WidgetData::WidgetData(std::unique_ptr<IWidget>& widget) :
			widget(std::move(widget)) {

			z = HVN3_DEFAULT_WIDGET_Z;
			removed = false;

		}
		IWidget& WidgetManager::WidgetData::GetRef() {
			return *widget;
		}


		WidgetManager::WidgetManager() :
			_dockable_region(0.0f, 0.0f) {

			_initialize();

		}
		WidgetManager::WidgetManager(IWidget* owner) :
			WidgetManager() {

			_owner = owner;
			_dockable_region = RectangleF(0.0f, 0.0f, owner->Width(), owner->Height());

		}
		WidgetManager::WidgetManager(renderer_ptr_type& renderer) :
			_renderer(std::move(renderer)),
			_dockable_region(0.0f, 0.0f) {

			_initialize();

		}

		void WidgetManager::AddChildManager(IWidget* parent, WidgetManager* manager) {

			_child_managers[parent] = manager;

		}
		void WidgetManager::RemoveChildManager(IWidget* parent) {

			auto it = _child_managers.find(parent);

			if (it == _child_managers.end())
				return;

			_child_managers.erase(it);

		}
		WidgetManager::widget_collection_type::iterator WidgetManager::begin() {
			return _widgets.begin();
		}
		WidgetManager::widget_collection_type::iterator WidgetManager::end() {
			return _widgets.end();
		}
		WidgetManager::widget_collection_type::const_iterator WidgetManager::begin() const {
			return _widgets.begin();
		}
		WidgetManager::widget_collection_type::const_iterator WidgetManager::end() const {
			return _widgets.end();
		}
		void WidgetManager::Add(IWidget* widget) {

			Add(std::unique_ptr<IWidget>(widget));

		}
		void WidgetManager::Add(std::unique_ptr<IWidget>& widget) {

			widget->SetManager(this);
			widget->SetParent(_owner);
			
			if (widget->HasChildren())
				widget->GetChildren().SetRenderer(GetRenderer());

			if (_owner != nullptr)
				_owner->OnChildWidgetAdded(ChildWidgetAddedEventArgs(_owner, widget.get()));

			// Don't allow the same widget to be added more than once.
			assert(std::find_if(_widgets.begin(), _widgets.end(), [&](const WidgetData& x) { return x.widget.get() == widget.get(); }) == _widgets.end());

			_widgets.emplace_back(WidgetData(widget));

		}
		void WidgetManager::Remove(IWidget* widget) {

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
				if (i->widget.get() == widget) {
					i->removed = true;
					break;
				}

		}
		void WidgetManager::Move(IWidget* widget, WidgetManager* to_manager) {

			// If trying to move a widget to a non-existent manager, just remove it.
			if (to_manager == nullptr) {
				Remove(widget);
				return;
			}

			// Get a pointer to the widget.
			auto iter = _findWidget(widget);

			if (iter == _widgets.end())
				return;

			// Move the widget data to the other manager. 
			to_manager->Add(std::move(iter->widget));
			widget->SetManager(to_manager);

			// Remove the pointer from this manager's collection.
			_widgets.erase(iter);

		}
		bool WidgetManager::Contains(IWidget* widget) const {

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
				if (i->widget.get() == widget)
					return true;

			return false;

		}
		const WidgetManager::widget_collection_type& WidgetManager::Widgets() const {
			return _widgets;
		}
		WidgetManager::widget_collection_type& WidgetManager::GetWidgets() {
			return _widgets;
		}
		WidgetManager::widget_collection_type::size_type WidgetManager::Count() const {
			return _widgets.size();
		}

		void WidgetManager::BringToFront(IWidget* widget) {

			auto iter = _findWidget(widget);

			if (iter == _widgets.end())
				return;

			iter->z = --_smallest_z_depth;
			_resort_required = true;

			widget->OnZDepthChanged(WidgetZDepthChangedEventArgs(widget));

		}
		void WidgetManager::SendToBack(IWidget* widget) {

			auto iter = _findWidget(widget);

			if (iter == _widgets.end())
				return;

			iter->z = _widgets.front().z + 1;
			_resort_required = true;

			widget->OnZDepthChanged(WidgetZDepthChangedEventArgs(widget));

		}

		void WidgetManager::SetRenderer(renderer_ptr_type& renderer) {

			_renderer = renderer;

			// Let all child widgets know that the renderer has changed.
			for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
				i->widget->OnRendererChanged(WidgetRendererChangedEventArgs(i->widget.get()));

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

			if (_update_required_before_draw)
				OnUpdate(UpdateEventArgs(0.0));

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i) {

				if (!i->widget->Visible())
					continue;

				// If the widget is the top-most modal dialog, we'll skip it for now (it will be drawn after all other widgets).
				if (_modal_dialogs.size() > 0 && _modal_dialogs.back() == i->widget.get())
					continue;

				_renderWidget(e, *i);

			}

			// Draw the top-most modal dialog.

			if (_modal_dialog_background_alpha != 0.0f) {

				e.Graphics().SetClip(DockableRegion());
				e.Graphics().DrawSolidRectangle(DockableRegion(), Color::FromArgbf(0, 0, 0, _modal_dialog_background_alpha));

			}

			if (_modal_dialogs.size() > 0 && _modal_dialogs.back()->Visible()) {

				for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
					if (i->widget.get() == _modal_dialogs.back()) {
						_renderWidget(e, *i);
						break;
					}

			}

		}
		void WidgetManager::OnUpdate(UpdateEventArgs& e) {

			if (_widget_hovered != nullptr)
				_widget_hovered->HandleEvent(WidgetMouseHoverEventArgs(_widget_hovered, _last_mouse_position, e.Delta()));

			RectangleF current_dockable_region = DockableRegion();

			// When widgets are drawn, they are automatically offset by the dockable region offset, and therefore do not need to be offset here.
			current_dockable_region.SetPosition(0.0f, 0.0f);

			bool pending_removals = false;

			for (auto i = _widgets.begin(); i != _widgets.end(); ++i) {

				if (i->removed)
					pending_removals = true;

				if (!i->widget->Visible())
					continue;

				// If the widget is docked, adjust its size and position accordingly.
				_applyDockStyle(i->widget.get(), current_dockable_region);

				// Call the update event for the widget.
				i->widget->HandleEvent(WidgetUpdateEventArgs(i->widget.get(), e.Delta()));

				// Call the update event for the widget's child widgets.
				if (i->widget->HasChildren())
					i->widget->GetChildren().OnUpdate(e);

				// Update the widget's transition data (i.e. update animations).
				i->rendererArgs.UpdateTransitionData(static_cast<float>(e.Delta()));

			}

			// If there are pending removals, handle them now.
			if (pending_removals) {
				_widgets.erase(std::remove_if(_widgets.begin(), _widgets.end(), [this](const WidgetData& x) {

					if (x.removed) {

						IWidget* ptr = x.widget.get();

						// Remove the widget from the list of modal widgets if present.
						_modal_dialogs.erase(std::remove(_modal_dialogs.begin(), _modal_dialogs.end(), ptr), _modal_dialogs.end());

						if (_widget_hovered == ptr)
							_widget_hovered = nullptr;

						if (_widget_held == ptr)
							_widget_held = nullptr;

						if (_widget_focused == ptr || (ptr->HasChildren() && ptr->GetChildren().Contains(_widget_focused)))
							_widget_focused = nullptr;

					}

					return x.removed;

				}), _widgets.end());
			}

			// If sorting is required, sort the list of widgets, and then reset their z values.

			if (_resort_required) {

				_widgets.sort([](const WidgetData& lhs, const WidgetData& rhs) {
					return lhs.z > rhs.z;
				});

				// Reset the z values after sorting so that BringToFront/SendToBack continue to work.
				for (auto i = _widgets.begin(); i != _widgets.end(); ++i)
					i->z = HVN3_DEFAULT_WIDGET_Z;

				_smallest_z_depth = HVN3_DEFAULT_WIDGET_Z;
				_resort_required = false;

			}

			_update_required_before_draw = false;

			// Update the background fade animation if we're showing a modal dialog.

			float overlay_max_alpha = 0.3f;
			float overlay_fade_speed = 4.0f;

			if (_modal_dialogs.size() > 0) {
				if (_modal_dialog_background_alpha < overlay_max_alpha)
					_modal_dialog_background_alpha = Math::Min(overlay_max_alpha, _modal_dialog_background_alpha + static_cast<float>(overlay_fade_speed * e.Delta()));
			}
			else if (_modal_dialog_background_alpha > 0.0f)
				_modal_dialog_background_alpha = Math::Max(0.0f, _modal_dialog_background_alpha - static_cast<float>(overlay_fade_speed * e.Delta()));

		}

		void WidgetManager::OnKeyDown(KeyDownEventArgs& e) {

			if (_widget_focused == nullptr)
				return;

			_widget_focused->HandleEvent(WidgetKeyDownEventArgs(_widget_focused, e));

		}
		void WidgetManager::OnKeyPressed(KeyPressedEventArgs& e) {

			if (_widget_focused == nullptr)
				return;

			_widget_focused->HandleEvent(WidgetKeyPressedEventArgs(_widget_focused, e));

		}
		void WidgetManager::OnKeyUp(KeyUpEventArgs& e) {

			if (_widget_focused == nullptr)
				return;

			_widget_focused->HandleEvent(WidgetKeyUpEventArgs(_widget_focused, e));

		}
		void WidgetManager::OnKeyChar(KeyCharEventArgs& e) {

			if (_widget_focused == nullptr)
				return;

			_widget_focused->HandleEvent(WidgetKeyCharEventArgs(_widget_focused, e));

		}
		void WidgetManager::OnKeyboardLost(KeyboardLostEventArgs& e) {}
		void WidgetManager::OnKeyboardFound(KeyboardFoundEventArgs& e) {}

		void WidgetManager::OnMouseDown(MouseDownEventArgs& e) {}
		void WidgetManager::OnMousePressed(MousePressedEventArgs& e) {

			// If no widget is hovered over, clear the focused widget.
			if (_widget_hovered == nullptr)
				_setFocused(nullptr);

			// Call the mouse-down event for the currently-hovered widget if the mouse was not held over a different widget.
			if (_widget_hovered != nullptr && _widget_held == nullptr) {

				_widget_hovered->HandleEvent(WidgetMousePressedEventArgs(_widget_hovered, e));

				if (_widget_hovered->HasChildren())
					_widget_hovered->GetChildren().OnMousePressed(e);

				_widget_held = _widget_hovered;

				if (!e.Handled())
					_setFocused(_widget_held);

				e.SetHandled(true);

			}

		}
		void WidgetManager::OnMouseReleased(MouseReleasedEventArgs& e) {

			// Only call the mouse-up event for a widget we've previously called the mouse-down event for.
			if (_widget_held != nullptr) {

				_widget_held->HandleEvent(WidgetMouseReleasedEventArgs(_widget_held, e));

				if (_widget_held->HasChildren())
					_widget_held->GetChildren().OnMouseReleased(e);

				// If the mouse was released on the same widget that it went down on, consider it a click.
				if (_widget_hovered == _widget_held)
					_widget_held->HandleEvent(WidgetMouseClickEventArgs(_widget_held, e));
				_widget_held = nullptr;

			}

		}
		void WidgetManager::OnMouseMove(MouseMoveEventArgs& e) {

			IWidget* new_widget_hovered = nullptr;
			RectangleF fixed_parent_region = DockableRegion();

			if (_owner != nullptr)
				fixed_parent_region.SetPosition(fixed_parent_region.Position() + _owner->FixedPosition());

			/*
			Find the widget that the mouse is currently hovering over.
			The widgets are iterated over such that the highest ones are checked first.
			This only needs to be done if the mouse is within the fixed region of the parent widget.
			*/

			if (Math::Geometry::PointIn(e.Position(), fixed_parent_region)) {
				for (auto i = _widgets.rbegin(); i != _widgets.rend(); ++i) {

					// If the widget is not visible, the user should not be able to interact with it.
					if (!i->widget->Visible())
						continue;

					// If there is a modal dialog visible, it should be the only widget that can be interacted with.
					if (_modal_dialogs.size() > 0 && i->widget.get() != _modal_dialogs.back())
						continue;

					// All widgets that the mouse touches should receive a mouse movement event.
					// (?) Was that the intent here, or should this be called for all widgets regardless?
					IWidget* widget = i->widget.get();
					widget->HandleEvent(WidgetMouseMoveEventArgs(widget, e));

					if (new_widget_hovered == nullptr && !e.Handled() && Math::Geometry::PointIn(e.Position(), widget->Bounds()))
						new_widget_hovered = widget;

				}
			}

			/*
			We now have the widget that the mouse is currently hovering over (which could be the same widget as the last check).
			*/

			if (new_widget_hovered != _widget_hovered) {

				if (_widget_hovered != nullptr) {

					e.SetHandled(true);

					_widget_hovered->HandleEvent(WidgetMouseLeaveEventArgs(_widget_hovered, e));

					// If the widget has children, give them the opportunity to handle the event as well.
					if (_widget_hovered->HasChildren())
						_widget_hovered->GetChildren().OnMouseMove(e);

					e.SetHandled(false);

				}

				if (new_widget_hovered != nullptr)
					new_widget_hovered->HandleEvent(WidgetMouseEnterEventArgs(new_widget_hovered, e));

			}

			// If the widget has children, give them the opportunity to handle the event as well.
			if (new_widget_hovered != nullptr && new_widget_hovered->HasChildren())
				new_widget_hovered->GetChildren().OnMouseMove(e);

			// Update the cursor to reflect the hovered widget.
			if (new_widget_hovered == nullptr)
				Mouse::SetCursor(SystemCursor::Default);
			else
				Mouse::SetCursor(new_widget_hovered->Cursor());

			_last_mouse_position = e.Position();
			_widget_hovered = new_widget_hovered;

		}
		void WidgetManager::OnMouseScroll(MouseScrollEventArgs& e) {}

		void WidgetManager::ShowDialog(std::unique_ptr<IWidget>& widget) {

			IWidget* ptr = widget.get(); // Allows access to widget pointer after move

			// The widget manager will manage the lifetime of the dialog widget.
			Add(widget);
			_modal_dialogs.push_back(ptr);

			// Center the widget in the middle of the dockable region.
			ptr->SetX(DockableRegion().X() + ((DockableRegion().Width() / 2.0f) - (ptr->Width() / 2.0f)));
			ptr->SetY(DockableRegion().Y() + ((DockableRegion().Height() / 2.0f) - (ptr->Height() / 2.0f)));

			ptr->BringToFront();

		}



		IWidget* WidgetManager::_widget_focused = nullptr;

		void WidgetManager::_initialize() {

			_widget_hovered = nullptr;
			_widget_held = nullptr;
			_resort_required = false;
			_owner = nullptr;
			_update_required_before_draw = true;
			_smallest_z_depth = HVN3_DEFAULT_WIDGET_Z;
			_modal_dialog_background_alpha = 0.0f;

		}
		WidgetManager::widget_collection_type::iterator WidgetManager::_findWidget(IWidget* widget) {
			return std::find_if(_widgets.begin(), _widgets.end(), [=](const WidgetData& x) { return x.widget.get() == widget; });
		}
		void WidgetManager::_renderWidget(DrawEventArgs& e, WidgetData& data) {

			Graphics::GraphicsState state = e.Graphics().Save();

			e.Graphics().SetClip(RectangleF::Intersection(data.widget->Bounds(), e.Graphics().Clip()));

			// Render the widget.
			_getRenderer()->DrawWidget(e.Graphics(), data.GetRef(), data.rendererArgs);
			data.widget->OnDraw(WidgetDrawEventArgs(data.widget.get(), &e));

			// Render the widget's child widgets.
			_renderChildWidgets(e, data.widget.get());

			e.Graphics().Restore(state);

		}
		void WidgetManager::_renderChildWidgets(DrawEventArgs& e, IWidget* widget) {

			if (!widget->HasChildren())
				return;

			Graphics::GraphicsState state = e.Graphics().Save();

			Graphics::Transform t = e.Graphics().GetTransform();
			t.Translate(widget->Position().x + widget->GetChildren().DockableRegion().X(),
				widget->Position().y + widget->GetChildren().DockableRegion().Y());

			e.Graphics().SetTransform(t);
			e.Graphics().SetClip(RectangleF(widget->FixedPosition() + widget->GetChildren().DockableRegion().Position(), widget->GetChildren().DockableRegion().Size()));

			widget->GetChildren().OnDraw(e);

			e.Graphics().Restore(state);

		}
		void WidgetManager::_applyDockStyle(IWidget* widget, RectangleF& region) {

			switch (widget->DockStyle()) {

			case DockStyle::Top:
				widget->SetPosition(region.Position());
				widget->SetWidth(region.Width());
				region = RectangleF::Crop(region, hvn3::CropSide::Top, widget->Height());
				break;

			case DockStyle::Bottom:
				widget->SetPosition(region.X(), region.Height() - widget->Height());
				widget->SetWidth(region.Width());
				region = RectangleF::Crop(region, hvn3::CropSide::Bottom, widget->Height());
				break;

			case DockStyle::Left:
				widget->SetPosition(region.Position());
				widget->SetHeight(region.Height());
				region = RectangleF::Crop(region, hvn3::CropSide::Left, widget->Width());
				break;

			case DockStyle::Right:
				widget->SetPosition(region.Width() - widget->Width(), region.Y());
				widget->SetHeight(region.Height());
				region = RectangleF::Crop(region, hvn3::CropSide::Right, widget->Width());
				break;

			case DockStyle::Fill:
				widget->SetPosition(region.Position());
				widget->SetSize(region.Size());
				break;

			default:
				break;

			}

		}
		void WidgetManager::_setFocused(IWidget* widget) {

			if (_widget_focused == widget)
				return;

			if (_widget_focused != nullptr)
				_widget_focused->HandleEvent(WidgetFocusLostEventArgs(_widget_focused));

			if (widget != nullptr)
				widget->HandleEvent(WidgetFocusEventArgs(widget));

			_widget_focused = widget;

		}

		WidgetManager::renderer_ptr_type& WidgetManager::_getRenderer() {

			if (!_renderer)
				SetRenderer(renderer_ptr_type(new DefaultWidgetRenderer));

			return _renderer;

		}

	}
}