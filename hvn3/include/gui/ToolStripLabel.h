#pragma once
#include "gui/ToolStripItem.h"
#include "gui/TextableControl.h"

namespace hvn3 {
	namespace Gui {

		class ToolStripLabel : public ToolStripItem, public TextableControl {

		public:
			ToolStripLabel(const String& text);

			virtual void OnPaint(PaintEventArgs& e) override;

		};

	}
}