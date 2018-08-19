#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class CheckBox :
			public WidgetBase {

		public:
			CheckBox(const String& text) {

				SetText(text);
				SetIdentifier("checkbox");

				_checkbox_width = 12.0f + 5.0f;

				SetPosition(0.0f, 50.0f);

			}

			void SetText(const String& value) override {

				WidgetBase::SetText(value);

				_updateSize();

			}
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override {

				WidgetBase::OnRendererChanged(e);

				_updateSize();

			}
			void OnMousePressed(WidgetMousePressedEventArgs& e) override {

				WidgetBase::OnMousePressed(e);

				SetState(WidgetState::Checked, !HasFlag(State(), WidgetState::Checked));
				
			}

		private:
			// The width of the checkbox + padding used to calculate the width of the widget. 
			// The renderer should use the same value.
			float _checkbox_width;


			void _updateSize() {

				IWidgetRenderer* r = GetRenderer();

				if (r == nullptr)
					return;

				SizeF string_size = r->MeasureString(Text());

				SetSize(string_size.width + _checkbox_width, Math::Max(string_size.height, _checkbox_width));

			}

		};

	}
}