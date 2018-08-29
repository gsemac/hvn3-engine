#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/utility/StringUtils.h"
#include <algorithm>

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

				_initializeText();

			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				float padding = 3.0f;

				e.Graphics().DrawText(X() + padding, Y() + Height() / 2.0f, Text(), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Left | Alignment::Middle);

				if (_caret_blink_timer <= 1.0f && _focused) {

					float caret_x = X() + _text_padding + _getCaretXOffset(_caret_pos);

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

				_initializeText();

			}
			void OnKeyChar(WidgetKeyCharEventArgs& e) override {

				if (Keyboard::IsPrintableChar(e.Key())) {

					if (_input_type == InputType::Numeric || _input_type == InputType::Decimal)
						if (!StringUtils::IsNumeric(e.CharCode()) && !(e.Char() == '.' && _input_type == InputType::Decimal && std::count(Text().begin(), Text().end(), '.') == 0))
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
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override {

				IWidgetRenderer* r = GetRenderer();

				if (r == nullptr)
					return;

				float h = Math::Max(r->MeasureString(Text()).Height(), 25.0f);

				SetHeight(h);

			}
			void OnMousePressed(WidgetMousePressedEventArgs& e) override {

				// Find the caret position closest to where the mouse was clicked.

				float click_x = e.Position().x;
				float best_distance = std::numeric_limits<float>::max();
				int best_pos = 0;

				for (int i = 0; static_cast<size_t>(i) < Text().Length() + 1; ++i) {

					float caret_x = FixedPosition().x + _text_padding + _getCaretXOffset(i);
					float dist = Math::Abs(caret_x - click_x);

					if (dist < best_distance) {

						best_distance = dist;
						best_pos = i;

					}
					else
						break;

				}

				_caret_pos = best_pos;
				_caret_blink_timer = 0.0f;

			}

		private:
			int _caret_pos;
			float _text_padding;
			float _caret_blink_timer;
			bool _focused;
			InputType _input_type;

			void _initializeMembers() {

				_caret_pos = 0;
				_text_padding = 3.0f;
				_caret_blink_timer = 0.0f;
				_focused = false;
				_input_type = InputType::Default;

			}
			void _initializeText() {

				switch (_input_type) {

				case InputType::Numeric:

					if (Text().Length() <= 0)
						SetText("0");

					break;

				case InputType::Decimal:

					if (Text().Length() <= 0)
						SetText("0.0");
					else {
						if (Text().StartsWith("."))
							SetText("0" + Text());
						if (Text().EndsWith("."))
							SetText(Text() + "0");
					}

					break;

				}

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
			float _getCaretXOffset(int caret_position) {

				float caret_x = 0.0f;

				if (GetRenderer() != nullptr)
					caret_x += GetRenderer()->MeasureString(Text().SubString(caret_position)).width;

				return caret_x;

			}

		};

	}
}