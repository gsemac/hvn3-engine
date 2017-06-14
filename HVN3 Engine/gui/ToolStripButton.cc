#include "gui/ToolStripButton.h"

namespace hvn3 {
	namespace Gui {

		ToolStripButton::ToolStripButton() :
			ButtonBase("ToolStripButton"),
			Control(PointF(0, 0), SizeF(25, 25)) {

		}

		void ToolStripButton::OnPaint(PaintEventArgs& e) {

			_RenderBase(e);

			switch (DisplayStyle()) {

			case ToolStripItemDisplayStyle::Image:
				_RenderImage(e);
				break;

			case ToolStripItemDisplayStyle::Text:
				_RenderText(e);
				break;

			}

		}

	}
}