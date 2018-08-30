#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Label :
			public WidgetBase {

		public:
			Label(const String& text) {

				SetId("label");
				SetText(text);
				SetTabStop(false);

			}
			Label(float x, float y, const String& text) :
				Label(text) {

				SetPosition(x, y);

			}

			void SetText(const String& value) override {

				WidgetBase::SetText(value);

				_updateSize();

			}
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override {

				WidgetBase::OnRendererChanged(e);

				_updateSize();

			}

		private:
			void _updateSize() {
			
				IWidgetRenderer* r = GetRenderer();

				if (r == nullptr)
					return;
				
				SetSize(r->MeasureString(Text()));

			}

		};

	}
}