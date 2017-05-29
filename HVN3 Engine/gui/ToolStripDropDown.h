#pragma once
#include "gui/ContainerControl.h"
#include "gui/ToolStripMenuItem.h"
#include <vector>

namespace hvn3 {
	namespace Gui {

		class ToolStripDropDown : public ContainerControl {

		public:
			ToolStripDropDown(const Point& position, float width);

			virtual void AddItem(ToolStripMenuItem* item);
			
			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			float _item_margin;
			
		};

	}
}