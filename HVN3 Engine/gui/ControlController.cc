#include "gui/ControlController.h"
#include "gui/Control.h"
#include "gui/GuiManager.h"
#include "gui/StyleManager.h"

namespace Gui {

	ControlController::ControlController(Control& control) :
		_control(&control) {
	}

	void ControlController::SetManager(GuiManager* manager) {

		// Contruct a manager changed event.
		ManagerChangedEventArgs e(_control->Manager());

		// Change the control's manager.
		_control->__manager = manager;

		// Trigger the event.
		_control->OnManagerChanged(e);

		// It's possible for the manager not to have a style manager at the moment (e.g., if it's a child manager).
		if (manager->StyleManager()) {
			_control->SetBackColor(manager->StyleManager()->PrimaryColor());
			_control->SetForeColor(manager->StyleManager()->SecondaryColor());
		}

	}
	Point ControlController::GetFixedPosition() const {

		return _control->GetFixedPosition();

	}
	Point ControlController::PreviousPosition() const {

		return _control->__previous_pos;

	}
	void ControlController::SetPreviousPosition(float x, float y) {

		_control->__previous_pos.SetXY(x, y);

	}
	bool ControlController::PrevFocus() const {

		return _control->__prev_focus;

	}
	void ControlController::SetPrevFocus(bool value) {

		_control->__prev_focus = value;

	}

}