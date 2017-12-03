#pragma once
#include "hvn3/gui/ContainerControl.h"
#include "hvn3/gui/ToolStripItem.h"

namespace hvn3 {
	namespace Gui {

		class ToolStrip : public ContainerControl {

		public:
			ToolStrip();

			virtual void AddItem(ToolStripItem* item);

			virtual void OnPaint(PaintEventArgs& e) override;

		private:

			
		};

	}
}