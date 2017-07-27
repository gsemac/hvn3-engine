#include "ToolStripDropDown.h"
#define DEFAULT_ITEM_MARGIN 0.0f

namespace hvn3 {
	namespace Gui {

		ToolStripDropDown::ToolStripDropDown(const PointF& position, float width) :
			Control(position, SizeF(width, 0.0f)) {

			_item_margin = DEFAULT_ITEM_MARGIN;


		}

		void ToolStripDropDown::AddItem(ToolStripMenuItem* item) {

			// Anchor the item to the sides of the dropdown, and resize it to match.
			item->SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT);
			item->SetWidth(Width());

			// Move the item into position.
			item->SetPosition(0, Height());

			// Resize the control to compensate for the new item.
			Resize(Width(), Height() + item->Height() + _item_margin);

			// Add the new item to the collection.
			_Controls()->AddControl(ControlPtr(item));

			// Add the new item to our list of items.
			_menu_items.push_back(item);

			// Invalidate the control so that's redrawn with the new item.
			Invalidate();

		}
		bool ToolStripDropDown::HasActiveSubMenu() const {

			for (size_t i = 0; i < _menu_items.size(); ++i)
				if (_menu_items[i]->HasActiveSubMenu())
					return true;

			return false;

		}
		bool ToolStripDropDown::HasVisibleSubMenu() const {

			for (size_t i = 0; i < _menu_items.size(); ++i)
				if (_menu_items[i]->SubMenu() && _menu_items[i]->SubMenu()->Visible())
					return true;

			return false;

		}

		void ToolStripDropDown::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());

			ContainerControl::OnPaint(e);

		}

		// Private methods

	}
}