#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/utility/StringUtils.h"

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
			TextBox(float width, InputType input_type) :
				TextBox(width) {

				_input_type = input_type;

			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				float padding = 3.0f;

				e.Graphics().DrawText(X() + padding, Y() + Height() / 2.0f, Text(), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Left | Alignment::Middle);

				if (_caret_blink_timer <= 1.0f && _focused) {

					float caret_x = X() + padding;

					if (GetRenderer() != nullptr)
						caret_x += GetRenderer()->MeasureString(Text().SubString(_caret_pos)).width;

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

				if (Keyboard::IsPrintableChar(e.Key())) {

					if (_input_type == InputType::Numeric)
						if (!StringUtils::IsNumeric(e.CharCode()))
							return;
				
						_insertAtCaret(e.CharCode());
				
				}
				else if (e.Key() == Key::Backspace || e.Key() == Key::Delete)
					_removeAtCaret(e.Key() == Key::Backspace);
				else if (e.Key() == Key::Left)
					_caret_pos = Math::Max(0, _caret_pos - 1);
				else if (e.Key() == Key::Right)
					_caret_pos = Math::Min<int>(Text().Length(), _caret_pos + 1);

				_caret_blink_timer = 0.0f;

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
			InputType _input_type;

			void _initializeMembers() {

				_caret_pos = 0;
				_caret_blink_timer = 0.0f;
				_input_type = InputType::Unrestricted;

			}
			void _insertAtCaret(int32_t character) {

				String text = Text();
				text.Insert(_caret_pos, character);

				SetText(text);

				++_caret_pos;

			}
			void _removeAtCaret(bool remove_left) {

				// Make sure that the character to remove is in the bounds of the string.

				if (remove_left) {
					if (_caret_pos <= 0)
						return;
				}
				else if (_caret_pos == Text().Length())
					return;

				// Generate a new string and update the widget's text.

				String new_str = Text();
				new_str.RemoveAt(static_cast<size_t>(remove_left ? _caret_pos - 1 : _caret_pos));

				SetText(new_str);

				// Update the caret position.

				if (remove_left)
					--_caret_pos;

			}

		};

	}
}