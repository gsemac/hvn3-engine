#include "gui/IContainer.h"
#include "gui/Control.h"

namespace Gui {

	IContainer::IContainer(Control* control) :
		_manager(&_control_manager, nullptr),
		_control_manager(&_manager, control),
		_control(control) {

	}

	Gui::ControlManager* IContainer::Controls() {

		SetUpStyleManager();

		// Return the control manager.
		return &_control_manager;

	}

	// Protected methods

	Gui::GuiManager* IContainer::ChildControlManager() {

		SetUpStyleManager();

		// Return the gui manager.
		return &_manager;

	}

	// Private methods

	void IContainer::SetUpStyleManager() {

		// Update the style manager of our gui manager to match the one used by the control.
		// Do this when controls are accessed so that we can be sure it's up-to-date before controls are accessed.
		if (_manager.StyleManager() == nullptr && _control->Manager() != nullptr)
			_manager = Gui::GuiManager(&_control_manager, _control->Manager()->StyleManager());

	}

	// Private member classes

	IContainer::ContainerControlManager::ContainerControlManager(GuiManager* gui_manager, Control* parent)
		: ControlManager(gui_manager),
		_control(parent) {
	}
	Handle<Control> IContainer::ContainerControlManager::AddControl(ControlPtr& control) {

		control->SetParent(_control);

		return ControlManager::AddControl(control);

	}

}