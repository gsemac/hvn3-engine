#pragma once
#include "ITextable.h"
#include "Control.h"

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
		Point _text_offset;

	};

}