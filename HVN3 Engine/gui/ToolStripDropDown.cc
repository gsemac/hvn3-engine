#include "ToolStripDropDown.h"
#define DEFAULT_ITEM_MARGIN 1.0f

namespace hvn3 {
	namespace Gui {

		ToolStripDropDown::ToolStripDropDown(const Point& position, float width) :
			Control(position, Size(width, 0.0f)) {

			_item_margin = DEFAULT_ITEM_MARGIN;

		}

		void ToolStripDropDown::AddItem(ToolStripMenuItem* item) {

			// Anchor the item to the sides of the dropdown, and resize it to match.
			item->SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT);
			item->SetWidth(Width());

			// Move the item into position.
			item->SetXY(0, Height());

			// Resize the control to compensate for the new item.
			Resize(Width(), Height() + item->Height() + _item_margin);

			// Add the new item to the collection.
			_Controls()->AddControl(Control::Create(item));
			
			// Invalidate the control so that's redrawn with the new item.
			Invalidate();

		}

		void ToolStripDropDown::OnPaint(PaintEventArgs& e) {
			
			e.Graphics().Clear(BackColor());

			ContainerControl::OnPaint(e);

		}

		// Private methods

	}
}