#pragma once
#include "ITextableControl.h"

namespace GUI {

	class Button : public ITextableControl {

	public:
		Button(float x, float y, float width, float height, const char* text);

		virtual void OnMouseEnter() override;
		virtual void OnMouseLeave() override;
		virtual void OnMouseDown() override;
		virtual void OnMouseUp() override;
		virtual void OnClick() override;
		virtual void OnPaint(PaintEventArgs& e) override;

	private:
		Point __text_offset;

	};

}