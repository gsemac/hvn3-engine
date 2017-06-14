#include "gui/ToolStripLabel.h"
#define DEFAULT_CHAR_WIDTH 10

namespace hvn3 {
	namespace Gui {

		ToolStripLabel::ToolStripLabel(const String& text) :
			TextableControl(text),
			Control(PointF(0, 0), SizeF(text.Length() * DEFAULT_CHAR_WIDTH, 25)) {
		}

		void ToolStripLabel::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());

			if (Font()) {
				float tx = Round(Width() / 2.0f - Text().Width(Font()) / 2.0f);
				float ty = Round(Height() / 2.0f - Text().Height(Font()) / 2.0f);
				e.Graphics().DrawText(tx + 1.0f, ty + 1.0f, Text(), Font(), Color::FromArgb(0, 0, 0, 128), Alignment::Center);
				e.Graphics().DrawText(tx, ty, Text(), Font(), Color::FromArgb(186, 186, 186), Alignment::Center);
			}

		}

	}
}