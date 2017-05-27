#pragma once
#include "gui/ContainerControl.h"

namespace hvn3 {
	namespace Gui {

		class ToolStrip : public ContainerControl {

		public:
			ToolStrip();

			virtual void OnPaint(PaintEventArgs& e) override;

		private:

			
		};

	}
}