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
			virtual void OnMouseDown(MouseEventArgs& e) override;
			virtual void OnMouseUp(MouseEventArgs& e) override;
			virtual void OnClick() override;
			virtual void OnPaint(PaintEventArgs& e) override;
			
		protected:
			virtual void _renderBase(PaintEventArgs& e);
			virtual void _renderText(PaintEventArgs& e);
			virtual void _renderImage(PaintEventArgs& e);

		private:
			PointF _text_offset;


		};

	}
}