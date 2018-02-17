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
			WidgetBase(position, size) {

			_initializeMembers();

			SetText(text);

		}

		void Button::OnUpdate(WidgetUpdateEventArgs& e) {
			//// When the mouse is no longer hovering over the button, fade it out.
			//if (!_mouse_is_hovering && _mouse_hover_hightlight_amount > 0) {
			//	GetStyle().backColor = GetStyle().backColor.Darker(0.5f * e.Delta());
			//	--_mouse_hover_hightlight_amount;
			//}
			//_mouse_is_hovering = false; // On the next update, this will be set back to true by OnMouseHover.
		}
		void Button::OnMouseDown(WidgetMouseEventArgs& e) {
			SetState(WidgetState::Active, true);
		}
		void Button::OnMouseHover(WidgetMouseHoverEventArgs& e) {
			//// Highlight the button when the mouse is hovering over it.
			//if (_mouse_hover_hightlight_amount < 10) {
			//	GetStyle().backColor = GetStyle().backColor.Lighter(0.5f * e.Delta());
			//	++_mouse_hover_hightlight_amount;
			//}
			//_mouse_is_hovering = true;
		}
		void Button::OnMouseUp(WidgetMouseEventArgs& e) {
			SetState(WidgetState::Active, false);
		}



		void Button::_initializeMembers() {

			_auto_size = false;
			//_mouse_is_hovering = false;
			//_mouse_hover_hightlight_amount = 0;

			SetIdentifier("button");

		}

	}
}