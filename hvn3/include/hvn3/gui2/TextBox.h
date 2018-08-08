#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class TextBox :
			public WidgetBase {

		public:
			TextBox(float x, float y, float width) :
				TextBox(width) {
				SetPosition(x, y);
			}
			TextBox(float width) {

				SetIdentifier("textbox");
				SetWidth(width);

				_initializeMembers();

			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				float padding = 3.0f;

				e.Graphics().DrawText(X() + padding, Y() + Height() / 2.0f, Text(), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Left | Alignment::Middle);

				if (_caret_blink_timer <= 1.0f && _focused)
					e.Graphics().DrawLine(X() + padding, Y() + padding, X() + padding, Y() + Height() - padding * 2.0f, Color::Black, 1.0f);

			}
			void OnUpdate(WidgetUpdateEventArgs& e) override {

				WidgetBase::OnUpdate(e);

				if (_caret_blink_timer < 2.0f)
					_caret_blink_timer += 1.8f * e.Delta();
				else
					_caret_blink_timer = 0.0f;

			}
			void OnFocus(WidgetFocusEventArgs& e) override {

				WidgetBase::OnFocus(e);

				_caret_blink_timer = 2.0f;
				_focused = true;

			}
			void OnFocusLost(WidgetFocusLostEventArgs& e) override {

				WidgetBase::OnFocusLost(e);

				_focused = false;

			}
			void OnKeyChar(WidgetKeyCharEventArgs& e) override {

				String text = Text();
				text.Append(e.CharCode());

				SetText(text);

			}
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) {

				IWidgetRenderer* r = GetRenderer();

				if (r == nullptr)
					return;

				float h = Math::Max(r->MeasureString(Text()).Height(), 25.0f);

				SetHeight(h);

			}

		private:
			int _caret_pos;
			float _caret_blink_timer;
			bool _focused;

			void _initializeMembers() {

				_caret_pos = 0;
				_caret_blink_timer = 0.0f;

			}

		};

	}
}