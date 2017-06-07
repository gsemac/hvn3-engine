#pragma once
#include "gui/ToolStripItem.h"

namespace hvn3 {
	namespace Gui {
		
		ToolStripItem::ToolStripItem() {

			_display_style = ToolStripItemDisplayStyle::ImageAndText;

		}

		ToolStripItemDisplayStyle ToolStripItem::DisplayStyle() const {

			return _display_style;

		}
		void ToolStripItem::SetDisplayStyle(ToolStripItemDisplayStyle style) {

			_display_style = style;

		}

	}
}