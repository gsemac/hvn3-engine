#pragma once
#include "hvn3/gui/ITextable.h"
#include "hvn3/gui/Control.h"

// #todo Use ButtonBase class and stop using deprecated ITextable (use TextableControl instead)

namespace hvn3 {

	namespace Gui {

		class Button : public Control, public ITextable {

		public:
			Button(float x, float y, float width, float height, const String& text);

			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseDown(MouseEventArgs& e) override;
			virtual void OnMouseUp(MouseEventArgs& e) override;
			virtual void OnClick() override;
			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			PointF _text_offset;

		};

	}

}