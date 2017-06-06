#include "gui/IContainer.h"
#include "gui/Control.h"
#include "io/Mouse.h"

namespace hvn3 {

	namespace Gui {

		IContainer::IContainer(Control* control) :
			_manager(Rectangle(0, 0, control->Width(), control->Height()), &_control_manager, nullptr),
			_control_manager(&_manager, control),
			_control(control),
			_child_region(control->Width(), control->Height()) {

		}

		Gui::ControlManager* IContainer::Controls() {

			SetUpStyleManager();

			return &_control_manager;

		}

		// Protected methods

		Gui::GuiManager* IContainer::ChildControlManager() {

			SetUpStyleManager();

			// Return the gui manager.
			return &_manager;

		}
		bool IContainer::HasActiveChildControl() const {

			return _control_manager.ActiveControl() != nullptr;

		}
		const Rectangle& IContainer::ChildRegion() const {

			return _child_region;

		}
		void IContainer::SetChildRegion(const Rectangle& region) {

			_child_region = region;

		}
		bool IContainer::MouseInChildRegion() const {

			// Get the child region and offset it to match the control's position.
			Rectangle region = _child_region;
			region.Translate(_control->FixedPosition().X(), _control->FixedPosition().Y());

			// Check if the mouse is in this region.
			return Mouse::InRegion(region);

		}
		void IContainer::UpdateAnchors(ResizeEventArgs& e) {

			// Calculate the difference in size.
			float width_diff = e.NewSize().Width() - e.OldSize().Width();
			float height_diff = e.NewSize().Height() - e.OldSize().Height();

			// Reposition all anchored Controls.
			for (auto it = Controls()->ControlsBegin(); it != Controls()->ControlsEnd(); ++it) {

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

		void IContainer::SetUpStyleManager() {

			// Update the style manager of our gui manager to match the one used by the control.
			// Do this when controls are accessed so that we can be sure it's up-to-date before controls are accessed.
			if (_manager.StyleManager() == nullptr && _control->Manager() != nullptr)
				_manager = Gui::GuiManager(ChildRegion(), &_control_manager, _control->Manager()->StyleManager());

		}

		// Private classes

		IContainer::ContainerControlManager::ContainerControlManager(GuiManager* gui_manager, Control* parent)
			: ControlManager(gui_manager),
			_control(parent) {
		}
		Handle<Control> IContainer::ContainerControlManager::AddControl(ControlPtr& control) {

			control->SetParent(_control);

			return ControlManager::AddControl(control);

		}

	}

}