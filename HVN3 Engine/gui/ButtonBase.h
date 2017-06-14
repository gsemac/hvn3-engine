#pragma once
#include "gui/TextableControl.h"
#include "gui/ImageableControl.h"

namespace hvn3 {
	namespace Gui {

		class ButtonBase : public virtual Control, public TextableControl, public ImageableControl {

		public:
			ButtonBase(const String& text);

			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseDown() override;
			virtual void OnMouseUp() override;
			virtual void OnClick() override;
			virtual void OnPaint(PaintEventArgs& e) override;
			
		protected:
			virtual void _RenderBase(PaintEventArgs& e);
			virtual void _RenderText(PaintEventArgs& e);
			virtual void _RenderImage(PaintEventArgs& e);

		private:
			PointF _text_offset;


		};

	}
}