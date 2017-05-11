#include "gui/IContainer.h"
#include "gui/Control.h"
#include "io/Mouse.h"

namespace Gui {

	IContainer::IContainer(Control* control) :
		_manager(&_control_manager, nullptr),
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

	// Private methods

	void IContainer::SetUpStyleManager() {

		// Update the style manager of our gui manager to match the one used by the control.
		// Do this when controls are accessed so that we can be sure it's up-to-date before controls are accessed.
		if (_manager.StyleManager() == nullptr && _control->Manager() != nullptr)
			_manager = Gui::GuiManager(&_control_manager, _control->Manager()->StyleManager());

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