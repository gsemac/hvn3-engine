#pragma once
#include "Point2d.h"

namespace hvn3 {

	namespace Gui {

		class Control;
		class GuiManager;

		class ControlController {

		public:
			ControlController(Control& control);

			void SetManager(GuiManager* manager);
			Point2F GetFixedPosition() const;
			Point2F PreviousPosition() const;
			void SetPreviousPosition(float x, float y);
			bool PrevFocus() const;
			void SetPrevFocus(bool value);

		private:
			Control* _control;

		};

	}

}