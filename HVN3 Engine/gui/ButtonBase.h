#pragma once
#include "gui/TextableControl.h"

namespace hvn3 {
	namespace Gui {

		class ButtonBase : public virtual Control, public TextableControl {

		public:
			ButtonBase(const String& text);

			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseDown() override;
			virtual void OnMouseUp() override;
			virtual void OnClick() override;
			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			Point _text_offset;

		};

	}
}