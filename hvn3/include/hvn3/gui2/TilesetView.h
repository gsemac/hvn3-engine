#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class TilesetView : public WidgetBase {

		public:
			TilesetView() : 
				WidgetBase(0.0f, 0.0f, 10.0f, 10.0f) {
				_color = Color::Red;
			}
			void OnDraw(WidgetDrawEventArgs& e) override {

				e.Graphics().DrawSolidRectangle(X(), Y(), Width(), Height(), _color);
				e.Graphics().DrawLine(X(), Y(), Width(), Height(), Color::Black, 3.0f);
				e.Graphics().DrawLine(Width(), Y(), X(), Height(), Color::Black, 3.0f);


			}
			void OnMouseEnter(WidgetMouseEnterEventArgs& e) override {
				WidgetBase::OnMouseEnter(e);

				_color = Color::Blue;

			}
			void OnMouseLeave(WidgetMouseLeaveEventArgs& e) override {
				WidgetBase::OnMouseLeave(e);

				_color = Color::Red;

			}

		private:
			Color _color;

		};

	}
}