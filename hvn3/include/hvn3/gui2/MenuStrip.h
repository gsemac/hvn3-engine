#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class MenuStrip : public WidgetBase {

		public:
			MenuStrip() {
				
				SetIdentifier("menustrip");
				SetDockStyle(DockStyle::Top);

			}

		private:

		};

	}
}