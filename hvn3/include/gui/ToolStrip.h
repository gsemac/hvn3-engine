#pragma once
#include "gui/ContainerControl.h"
#include "gui/ToolStripItem.h"

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