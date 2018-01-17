#include "hvn3/gui/ControlController.h"
#include "hvn3/gui/Control.h"
#include "hvn3/gui/_GuiManager.h"
#include "hvn3/gui/StyleManager.h"

namespace hvn3 {

	namespace Gui {

		namespace System {

			ControlController::ControlController(Control& control) :
				_control(&control) {
			}

			void ControlController::SetManager(GuiManager* manager) {

				// Contruct a manager changed event.
				ManagerChangedEventArgs e(_control->Manager());

				// Change the control's manager.
				_control->_manager = manager;

				// Trigger the event.
				_control->OnManagerChanged(e);

				// It's possible for the manager not to have a style manager at the moment (e.g., if it's a child manager).
				if (manager->StyleManager()) {
					_control->SetBackColor(manager->StyleManager()->PrimaryColor());
					_control->SetForeColor(manager->StyleManager()->TextColor());
				}

			}
			PointF ControlController::GetFixedPosition() const {

				return _control->GetFixedPosition();

			}
			PointF ControlController::PreviousPosition() const {

				return _control->_previous_pos;

			}
			void ControlController::SetPreviousPosition(float x, float y) {

				_control->_previous_pos = PointF(x, y);

			}
			bool ControlController::PrevFocus() const {

				return _control->_prev_focus;

			}
			void ControlController::SetPrevFocus(bool value) {

				_control->_prev_focus = value;

			}

		}

	}

}