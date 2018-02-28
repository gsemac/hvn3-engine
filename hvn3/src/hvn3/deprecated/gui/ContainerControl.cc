#include "hvn3/gui/ContainerControl.h"
#include "hvn3/gui/ControlController.h"
#include "hvn3/io/Mouse.h"

namespace hvn3 {
	namespace Gui {

		ContainerControl::ContainerControl() :
			_gui_manager(&_control_manager, nullptr),
			_control_manager(&_gui_manager, this, RectangleF(Width(), Height())),
			_child_region(Width(), Height()) {
		}

		void ContainerControl::OnPaint(PaintEventArgs& e) {

			// Draw child controls.
			_controls()->OnDraw(e);

		}
		void ContainerControl::OnUpdate(UpdateEventArgs& e) {

			// Update child controls.
			//if (IsActiveControl())
			_controls()->OnUpdate(e);
			if (!IsActiveControl())
				_controls()->ClearActiveControl();

		}

		// Protected methods

		ControlManager* ContainerControl::_controls() {

			// Make sure that the style manager has been properly initialized before allowing access to child controls.
			_initializeStyleManager();

			// Return the underlying control manager.
			return &_control_manager;

		}
		GuiManager* ContainerControl::_childControlManager() {

			_initializeStyleManager();

			// Return the gui manager.
			return &_gui_manager;

		}
		bool ContainerControl::_hasActiveChildControl() const {

			return _control_manager.ActiveControl() != nullptr;

		}
		const RectangleF& ContainerControl::_childRegion() const {

			return _child_region;

		}
		void ContainerControl::_setChildRegion(const RectangleF& region) {

			_child_region = region;

		}
		bool ContainerControl::_mouseInChildRegion() const {

			// Get the child region and offset it to match the control's position.
			RectangleF region = _child_region;
			region.Translate(FixedPosition().X(), FixedPosition().Y());

			// Check if the mouse is in this region.
			return Mouse::InRegion(region);

		}
		void ContainerControl::_updateAnchors(ResizeEventArgs& e) {

			// Calculate the difference in size.
			float width_diff = e.NewSize().Width() - e.OldSize().Width();
			float height_diff = e.NewSize().Height() - e.OldSize().Height();

			// Reposition all anchored Controls.
			for (auto it = _controls()->ControlsBegin(); it != _controls()->ControlsEnd(); ++it) {

				Control* c = it->get();

				if (c->Anchors() & ANCHOR_RIGHT) {
					if (c->Anchors() & ANCHOR_LEFT)
						c->Resize(c->Width() + width_diff, c->Height());
					else
						c->SetPosition(c->X() + width_diff, c->Y());
				}

				if (c->Anchors() & ANCHOR_BOTTOM) {
					if (c->Anchors() & ANCHOR_TOP)
						c->Resize(c->Width(), c->Height() + height_diff);
					else
						c->SetPosition(c->X(), c->Y() + height_diff);
				}

				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_RIGHT) && !(c->Anchors() & ANCHOR_LEFT)))
					c->SetPosition(c->X() + width_diff / 2.0f, c->Y());

				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_TOP) && !(c->Anchors() & ANCHOR_BOTTOM)))
					c->SetPosition(c->X(), c->Y() + height_diff / 2.0f);

			}

		}

		// Private methods

		void ContainerControl::_initializeStyleManager() {

			// Updates the style manager of our gui manager so that uses the same one that's used by this control.
			if (_gui_manager.StyleManager() == nullptr && Manager() != nullptr && Manager()->StyleManager() != nullptr) {
			
				_gui_manager = gui_manager_type(&_control_manager, Manager()->StyleManager());

				// Call the manager changed event for all child controls.
				for (auto it = _controls()->ControlsBegin(); it != _controls()->ControlsEnd(); ++it)
					System::ControlController(*(*it).get()).SetManager(&_gui_manager);

			}
				
		}

		// Child classes

		ContainerControl::ContainerControlControlManager::ContainerControlControlManager(GuiManager* gui_manager, Control* parent, const RectangleF& dockable_region) :
			ControlManager(gui_manager, dockable_region) {

			_control = parent;

		}
		Handle<Control> ContainerControl::ContainerControlControlManager::AddControl(ControlPtr& control) {

			control->SetParent(_control);

			return ControlManager::AddControl(control);

		}

	}
}