#pragma once
#include "gui/ContainerControl.h"
#include "gui/ToolStripMenuItem.h"
#include <vector>

namespace hvn3 {
	namespace Gui {

		class ToolStripDropDown : public ContainerControl {

		public:
			ToolStripDropDown(const PointF& position, float width);

			virtual void AddItem(ToolStripMenuItem* item);
			bool HasActiveSubMenu() const;
			bool HasVisibleSubMenu() const;

			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			std::vector<ToolStripMenuItem*> _menu_items;
			float _item_margin;
			
		};

	}
}