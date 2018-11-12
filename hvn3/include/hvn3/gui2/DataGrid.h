#pragma once
#include "hvn3/gui2/ScrollableWidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class DataGrid :
			public ScrollableWidgetBase {

		public:
			DataGrid() {

				SetId("datagrid");
			
			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				e.Graphics().DrawSolidRectangle(X(), Y(), Width(), Height(), Color::White);

			}

		private:


		};

	}
}