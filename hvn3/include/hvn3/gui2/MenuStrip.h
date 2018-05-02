#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class MenuStrip : public WidgetBase {

		public:
			MenuStrip() : 
				MenuStrip(25.0f) {}
			MenuStrip(float height) {

				SetIdentifier("menustrip");
				SetDockStyle(DockStyle::Top);
				SetHeight(height);

			}

		private:

		};

	}
}