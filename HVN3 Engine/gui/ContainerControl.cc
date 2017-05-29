#include "gui/ContainerControl.h"
#include "io/Mouse.h"

namespace hvn3 {
	namespace Gui {

		ContainerControl::ContainerControl() :
			_gui_manager(&_control_manager, nullptr),
			_control_manager(&_gui_manager, this),
			_child_region(Width(), Height()) {
		}

		void ContainerControl::OnPaint(PaintEventArgs& e) {

			_Controls()->Draw(e);

		}

		// Protected methods

		ControlManager* ContainerControl::_Controls() {

			// Make sure that the style manager has been properly initialized before allowing access to child controls.
			_InitializeStyleManager();

			// Return the underlying control manager.
			return &_control_manager;

		}
		GuiManager* ContainerControl::_ChildControlManager() {

			_InitializeStyleManager();

			// Return the gui manager.
			return &_gui_manager;

		}
		bool ContainerControl::_HasActiveChildControl() const {

			return _control_manager.ActiveControl() != nullptr;

		}
		const Rectangle& ContainerControl::_ChildRegion() const {

			return _child_region;

		}
		void ContainerControl::_SetChildRegion(const Rectangle& region) {

			_child_region = region;

		}
		bool ContainerControl::_MouseInChildRegion() const {

			// Get the child region and offset it to match the control's position.
			Rectangle region = _child_region;
			region.Translate(FixedPosition().X(), FixedPosition().Y());

			// Check if the mouse is in this region.
			return Mouse::InRegion(region);

		}
		void ContainerControl::_UpdateAnchors(ResizeEventArgs& e) {

			// Calculate the difference in size.
			float width_diff = e.NewSize().Width() - e.OldSize().Width();
			float height_diff = e.NewSize().Height() - e.OldSize().Height();

			// Reposition all anchored Controls.
			for (auto it = _Controls()->ControlsBegin(); it != _Controls()->ControlsEnd(); ++it) {

				Control* c = it->get();

				if (c->Anchors() & ANCHOR_RIGHT) {
					if (c->Anchors() & ANCHOR_LEFT)
						c->Resize(c->Width() + width_diff, c->Height());
					else
						c->TranslateX(width_diff);
				}

				if (c->Anchors() & ANCHOR_BOTTOM) {
					if (c->Anchors() & ANCHOR_TOP)
						c->Resize(c->Width(), c->Height() + height_diff);
					else
						c->TranslateY(height_diff);
				}

				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_RIGHT) && !(c->Anchors() & ANCHOR_LEFT)))
					c->TranslateX(width_diff / 2.0f);

				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_TOP) && !(c->Anchors() & ANCHOR_BOTTOM)))
					c->TranslateY(height_diff / 2.0f);

			}

		}

		// Private methods

		void ContainerControl::_InitializeStyleManager() {

			// Updates the style manager of our gui manager so that uses the same one that's used by this control.
			if (_gui_manager.StyleManager() == nullptr && Manager() != nullptr && Manager()->StyleManager() != nullptr)
				_gui_manager = gui_manager_type(&_control_manager, Manager()->StyleManager());
		}

		// Child classes

		ContainerControl::ContainerControlControlManager::ContainerControlControlManager(GuiManager* gui_manager, Control* parent) :
			ControlManager(gui_manager) {

			_control = parent;

		}
		Handle<Control> ContainerControl::ContainerControlControlManager::AddControl(ControlPtr& control) {

			control->SetParent(_control);

			return ControlManager::AddControl(control);

		}

	}
}