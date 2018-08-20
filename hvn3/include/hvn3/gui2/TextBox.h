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

				if (_caret_blink_timer <= 1.0f && _focused) {

					float caret_x = X() + padding;

					if (GetRenderer() != nullptr)
						caret_x += GetRenderer()->MeasureString(Text().SubString(caret_x)).width;

					e.Graphics().DrawLine(caret_x, Y() + padding, caret_x, Y() + Height() - padding * 2.0f, Color::Black, 1.0f);

				}

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

				++_caret_pos;
				_caret_blink_timer = 0.0f;

			}
			void OnKeyDown(WidgetKeyDownEventArgs& e) override {

				WidgetBase::OnKeyDown(e);

				bool allow_press = false;

				if (_key_hold_timer == 0.0f)
					allow_press = true;

				if (_key_hold_timer < 1.0f)
					_key_hold_timer += 0.01f;
				else
					allow_press = true;

				if (!allow_press)
					return;

				if (e.Key() == Key::Backspace) {

					if (_caret_pos < 0)
						return;

					String new_str = Text();

					new_str.RemoveAt(static_cast<size_t>(_caret_pos));

					SetText(new_str);

					--_caret_pos;

				}

			}
			void OnKeyUp(WidgetKeyUpEventArgs& e) override {

				WidgetBase::OnKeyUp(e);

				_key_hold_timer = 0.0f;

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
			float _key_hold_timer;
			Key _last_key;
			bool _focused;

			void _initializeMembers() {

				_caret_pos = 0;
				_caret_blink_timer = 0.0f;
				_key_hold_timer = 0.0f;
				_last_key = Key::Unknown;

			}

		};

	}
}