#include "hvn3/gui/Button.h"
#include "hvn3/gui/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		Button::Button(const String& text) :
			Button(0.0f, 0.0f, text) {
		}
		Button::Button(float x, float y, const String& text)
			: Button(PointF(x, y), text) {
		}
		Button::Button(const PointF& position, const String& text) :
			Button(position, SizeF(0.0f, 0.0f), text) {

			_initializeMembers();

			_auto_size = true;

		}
		Button::Button(float x, float y, float width, float height, const String& text) :
			Button(PointF(x, y), SizeF(width, height), text) {
		}
		Button::Button(const PointF& position, const SizeF& size, const String& text) :
			WidgetBase(position, size) {

			_initializeMembers();

			SetText(text);

		}
		void Button::OnRendererChanged(WidgetRendererChangedEventArgs& e) {

			if (!_auto_size)
				return;

			if (GetManager() == nullptr)
				return;

			if (!const_cast<const WidgetManager*>(GetManager())->Renderer())
				return;

			SizeF size = GetManager()->Renderer()->MeasureString(Text());
			size.width = Math::Max(size.width, 25.0f);
			size.height = Math::Max(size.height, 25.0f);

			// Don't adjust the dimension if the user has already set it manually.

			if (Size().width <= 0.0f)
				SetWidth(size.width);

			if (Size().height <= 0.0f)
				SetHeight(size.height);

		}

		void Button::_initializeMembers() {

			_auto_size = false;

			SetId("button");

		}

	}
}