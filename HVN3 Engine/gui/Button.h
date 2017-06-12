#pragma once
#include "ITextable.h"
#include "Control.h"

// #todo Use ButtonBase class and stop using deprecated ITextable (use TextableControl instead)

namespace hvn3 {

	namespace Gui {

		class Button : public Control, public ITextable {

		public:
			Button(float x, float y, float width, float height, const String& text);

			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseDown() override;
			virtual void OnMouseUp() override;
			virtual void OnClick() override;
			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			Point2F _text_offset;

		};

	}

}