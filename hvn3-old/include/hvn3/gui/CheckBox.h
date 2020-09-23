#pragma once
#include "hvn3/gui/IWidgetRenderer.h"
#include "hvn3/gui/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class CheckBox :
			public WidgetBase {

		public:
			CheckBox(const String& text) {

				SetText(text);
				SetId("checkbox");

				_checkbox_width = 12.0f + 5.0f;

				SetPosition(0.0f, 50.0f);

			}

			bool Checked() const {
				return HasFlag(State(), WidgetState::Checked);
			}
			void SetChecked(bool value) {

				if (value == Checked())
					return;

				SetState(WidgetState::Checked, value);

				EmitEvent(WidgetCheckedStateChangedEventArgs(this, value));

			}

			void SetText(const String& value) override {

				WidgetBase::SetText(value);

				_updateSize();

			}
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override {

				WidgetBase::OnRendererChanged(e);

				_updateSize();

			}
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override {

				WidgetBase::OnMouseReleased(e);

				SetChecked(!Checked());

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