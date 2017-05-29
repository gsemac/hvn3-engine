#include "gui/ToolStripMenuItem.h"

namespace hvn3 {
	namespace Gui {

		ToolStripMenuItem::ToolStripMenuItem() :
			TextableControl("ToolStripMenuItem"),
			Control(Point(0, 0), Size(100, 25)) {



		}

		void ToolStripMenuItem::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(Color::Red);

			if (Font())
				e.Graphics().DrawText(0, 0, Text(), *Font(), Color::White);

		}

	}
}