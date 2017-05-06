#include "ControlController.h"
#include "Control.h"
#include "GuiManager.h"

namespace Gui {

	ControlController::ControlController(Control& control) :
		_control(&control) {
	}

	void ControlController::SetManager(GuiManager& manager) {

		_control->__manager = &manager;

		_control->SetBackColor(manager.StyleManager()->PrimaryColor());
		_control->SetForeColor(manager.StyleManager()->SecondaryColor());

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