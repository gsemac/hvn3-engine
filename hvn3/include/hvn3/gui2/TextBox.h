#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class TextBox :
			public WidgetBase {

		public:
			TextBox(float width, const String& text) {
				
				SetIdentifier("textbox");
				SetWidth(width);
				SetText(text);

			}

			void OnRendererChanged(WidgetRendererChangedEventArgs& e) {

				IWidgetRenderer* r = GetRenderer();

				if (r == nullptr)
					return;

				float h = Math::Max(r->MeasureString(Text()).Height(), 23.0f);

				SetHeight(h);
				
			}

		};

	}
}