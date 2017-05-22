#pragma once
#include "Point.h"

namespace hvn3 {

	namespace Gui {

		class Control;
		class GuiManager;

		class ControlController {

		public:
			ControlController(Control& control);

			void SetManager(GuiManager* manager);
			Point GetFixedPosition() const;
			Point PreviousPosition() const;
			void SetPreviousPosition(float x, float y);
			bool PrevFocus() const;
			void SetPrevFocus(bool value);

		private:
			Control* _control;

		};

	}

}