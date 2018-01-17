#include "hvn3/gui2/Button.h"

namespace hvn3 {
	namespace Gui {

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
		Button::Button(const PointF& position, const SizeF& size, const const String& text) :
			WidgetBase(position, size),
			_text(text) {

			_initializeMembers();

			_auto_size = false;

		}



		void Button::_initializeMembers() {

			_auto_size = false;

			SetName("button");

		}

	}
}