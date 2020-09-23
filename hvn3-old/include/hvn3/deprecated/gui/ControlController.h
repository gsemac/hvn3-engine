#pragma once
#include "hvn3/math/Point2d.h"

namespace hvn3 {
	namespace Gui {

		class Control;
		class GuiManager;

		namespace System {

			class ControlController {

			public:
				ControlController(Control& control);

				void SetManager(GuiManager* manager);
				PointF GetFixedPosition() const;
				PointF PreviousPosition() const;
				void SetPreviousPosition(float x, float y);
				bool PrevFocus() const;
				void SetPrevFocus(bool value);

			private:
				Control* _control;

			};

		}
	}
}