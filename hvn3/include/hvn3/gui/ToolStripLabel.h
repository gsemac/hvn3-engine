#pragma once
#include "hvn3/gui/ToolStripItem.h"
#include "hvn3/gui/TextableControl.h"

namespace hvn3 {
	namespace Gui {

		class ToolStripLabel : public ToolStripItem, public TextableControl {

		public:
			ToolStripLabel(const String& text);

			virtual void OnPaint(PaintEventArgs& e) override;

		};

	}
}