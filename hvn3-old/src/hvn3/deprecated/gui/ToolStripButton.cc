#include "hvn3/gui/ToolStripButton.h"

namespace hvn3 {
	namespace Gui {

		ToolStripButton::ToolStripButton() :
			ButtonBase("ToolStripButton"),
			Control(PointF(0, 0), SizeF(25, 25)) {

		}

		void ToolStripButton::OnPaint(PaintEventArgs& e) {

			_renderBase(e);

			switch (DisplayStyle()) {

			case ToolStripItemDisplayStyle::Image:
				_renderImage(e);
				break;

			case ToolStripItemDisplayStyle::Text:
				_renderText(e);
				break;

			}

		}

	}
}