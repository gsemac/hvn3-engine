#pragma once
#include <algorithm>
#include <allegro5/allegro.h>
#include "gui/Control.h"
#include "io/Clipboard.h"
#include "Range.h"
#include "StringUtils.h"
#include "Utility.h"
#define DEF_BORDER_RADIUS 3.0f

/*
Todo:
- Word-by-word drag selection
- Implement undo/redo buffer
*/

namespace hvn3 {

	namespace Gui {

		class Textbox : public Control, public ITextable {
			friend class Caret;

		public:
			Textbox(float x, float y, float width, float height) : Control(Point(x, y), Size(width, height)),
				__caret(this),
				__sel(0, 0),
				__sel_draw_pos(0.0f, 0.0f),
				__mouse_last_pos(Mouse::X, Mouse::Y),
				ITextable(this, "") {

				__highlight_color = Color(27, 161, 226);
				__padding = 3.0f;
				__hold_timer = 0.0f;
				__mouse_selecting = false;
				__full_word_selecting = false;

				__character_casing = StringUtils::CharacterCasing::Default;
				__accepts_tab = false;
				__numeric_only = false;
				__use_password_char = false;
				__password_char = '*';

			}
			String SelectedText() {

				return Text().SubString(__sel.Min(), __sel.Max());

			}

			void AppendText(const char* text) {

				RefText().Append(text);

				Invalidate();

			}
			void AppendText(int32_t ch) {

				RefText().Append(ch);

				Invalidate();

			}

			void Select(int start, int end) {

				// Set the selection area.
				__sel.Resize(start, end);

				// Set the selection bounds.
				int start_x = Text().RefSubString(0, start).Width(*Font());
				int end_x = start_x + Text().RefSubString(start, end).Width(*Font());
				__sel_draw_pos.Resize(start_x, end_x);

				// Hide the caret.
				__caret.Hide();

				// Invalidate the Control to show new area.
				Invalidate();

			}
			int SelectionStart() {

				return __sel.Min();

			}
			int SelectionLength() {

				return __sel.Length();

			}
			void SelectAll() {

				// Select all text.
				Select(0, (int)Text().Length());

			}
			void DeselectAll() {

				__sel.Resize(__caret.Position(), __caret.Position());
				__sel_draw_pos.Resize(__caret.X, __caret.X);

			}

			void ScrollToCaret() {

				// Clamp the offset within an acceptable range.
				__scroll_h_offset = Clamp((float)__scroll_h_offset, (std::min)(0.0f, -(Text().Width(*Font()) + __padding * 2 - Width())), 0.0f);

				// Calculate the visible region of text and the (actual) caret position.
				Range<float> visible_region(-__scroll_h_offset, -__scroll_h_offset + Width(), false);
				float caret_x = __caret.X + __padding;

				// If the caret is already within this range, do nothing.
				if (visible_region.ContainsValue(caret_x)) return;

				if ((Keyboard::LastChar() && !(__caret.Position() == Text().Length()))) {
					// If a key was pressed and the cursor is not at the end of the textbox, scroll the textbox such that the caret is in the middle of the textbox (or at the end).
					int diff_to_end = (Text().Width(*Font()) + __padding * 2 - Width()) + __scroll_h_offset;
					__scroll_h_offset -= (std::min)(visible_region.Length() / 2, diff_to_end);

				}
				else if (Keyboard::KeyDown(ALLEGRO_KEY_BACKSPACE)) {
					// Function similarily to when a key is pressed, except moving left instead of right.
					__scroll_h_offset += (std::min)(visible_region.Length() / 2, -__scroll_h_offset);
				}
				else {
					// If the caret is to the right the visible region, move region right.
					if (caret_x >= visible_region.End())
						__scroll_h_offset -= (caret_x - visible_region.End()) + __padding;

					// If the caret is to the left of the visible region, move region left.
					else if (caret_x <= visible_region.Start())
						__scroll_h_offset += (visible_region.Start() - caret_x) + __padding;
					// If pasting in front of text, scroll the textbox such that the caret is in the middle of the textbox (or at the end).
					if (Keyboard::KeyDown(ALLEGRO_KEY_LCTRL) && Keyboard::KeyDown(ALLEGRO_KEY_V) && !(__caret.Position() == Text().Length())) {
						int diff_to_end = (Text().Width(*Font()) + __padding * 2 - Width()) + __scroll_h_offset;
						__scroll_h_offset -= (std::min)(visible_region.Length() / 2, diff_to_end);
					}
				}

			}

			int GetCharacterIndexFromPoint(Point point) {

				// Initialize variables that we'll need.
				static const int ERROR_THRESHOLD = 3;
				Point real = FixedPosition();
				int width = Text().Width(*Font());
				int text_x1 = real.X() + __scroll_h_offset + __padding;
				int text_x2 = text_x1 + width;

				// If the string is empty, return first index.
				if (String::IsNullOrEmpty(Text())) return 0;

				// If the point is out of the textbox bounds, return first index.
				if (!PointIn(point, Rectangle(real.X(), real.Y(), Width(), Height()))) return 0;

				// If the point is beyond the position of the string, return the last index.
				if (point.X() > text_x2) return (int)Text().Length();

				// To select the best starting position, figure out what percentage down the string width our point is.
				double percent = (point.X() - text_x1) / width;
				int pos = (std::round)(Text().Length() * percent);
				int pos_x = text_x1 + Text().RefSubString(0, pos).Width(*Font());
				int err = point.X() - pos_x;
				int aerr = (std::abs)(err);

				// If the error is above the threshold, keep attempting to correct it until it can no longer be improved.
				if (aerr >= ERROR_THRESHOLD) {
					int besterr = aerr;
					int new_pos, new_pos_x, new_err;
					do {
						new_pos = pos + Signum(err);
						new_pos_x = text_x1 + Text().RefSubString(0, new_pos).Width(*Font());
						new_err = (std::abs)(point.X() - new_pos_x);
						if (new_err < besterr) {
							pos = new_pos;
							besterr = new_err;
						}
						else break;
					} while (1);
				}

				// Return the result.
				return pos;

			}

			bool AcceptsTab() {

				return __accepts_tab;

			}
			void SetAcceptsTab(bool value) {

				__accepts_tab = value;

			}
			bool NumericOnly() {

				return __numeric_only;

			}
			void SetNumericOnly(bool value) {

				__numeric_only = value;

			}
			StringUtils::CharacterCasing CharacterCasing() {

				return __character_casing;

			}
			void  SetCharacterCasing(StringUtils::CharacterCasing value) {

				// Convert the text into the new casing (if applicable).
				switch (value) {
				case StringUtils::CharacterCasing::Lower:
					SetText(Text().ToLower()); break;
				case StringUtils::CharacterCasing::Upper:
					SetText(Text().ToUpper()); break;
				}

				__character_casing = value;

			}
			float Padding() {

				return __padding;

			}
			void SetPadding(float value) {

				__padding = value;

			}
			int PasswordChar() {

				return __password_char;

			}
			void SetPasswordChar(int value) {

				__password_char = value;

			}
			bool UsePasswordChar() {

				return __use_password_char;

			}
			void SetUsePasswordChar(bool value) {

				__use_password_char = value;

			}

			int Lines() {

				return 1;

			}

			virtual void OnKeyPressed() override {
				if (!HasFocus()) return;

				// Show the caret.
				__caret.Show();

				// Reset the repeat timer.
				__hold_timer = 0.0f;

				// Handle keyboard shortcuts.
				if (Keyboard::KeyDown(KEY_CONTROL)) {

					// Select All (Ctrl + A)
					if (Keyboard::KeyPressed(ALLEGRO_KEY_A))
						SelectAll();

					// Copy (Ctrl + C)
					else if (Keyboard::KeyPressed(ALLEGRO_KEY_C))
						Clipboard::SetText(SelectedText());

					// Cut (Ctrl + X)
					else if (Keyboard::KeyPressed(ALLEGRO_KEY_X)) {
						Clipboard::SetText(SelectedText());
						RemoveSelectedText();
					}

					// Move Caret To Beginning Of Text (Ctrl + Home)
					else if (Keyboard::KeyPressed(ALLEGRO_KEY_HOME)) {
						DeselectAll();
						__caret.SetPosition(0);
					}

					// Move Caret To End Of Text (Ctrl + End)
					else if (Keyboard::KeyPressed(ALLEGRO_KEY_END)) {
						DeselectAll();
						__caret.SetPosition((int)Text().Length());
					}

				}
				else if (Keyboard::KeyDown(KEY_SHIFT)) {

					// Select Up To Caret (Shift + Up / Shift + Home)
					if (Keyboard::KeyPressed(ALLEGRO_KEY_UP) || Keyboard::KeyPressed(ALLEGRO_KEY_HOME)) {
						Select(0, __sel.Start());
						ScrollTo(0);
					}

					// Select From Caret To End (Shift + Down / Shift + End)
					if (Keyboard::KeyPressed(ALLEGRO_KEY_DOWN) || Keyboard::KeyPressed(ALLEGRO_KEY_END)) {
						Select(__sel.Start(), (int)Text().Length());
						ScrollTo(__sel.Max());
					}

				}
				else {

					// Move Caret To Beginning Of Text (Up)
					if (Keyboard::KeyPressed(ALLEGRO_KEY_UP) || Keyboard::KeyPressed(ALLEGRO_KEY_HOME)) {
						DeselectAll();
						__caret.SetPosition(0);
					}

					// Move Caret To End Of Text (Down)
					else if (Keyboard::KeyPressed(ALLEGRO_KEY_DOWN) || Keyboard::KeyPressed(ALLEGRO_KEY_END)) {
						DeselectAll();
						__caret.SetPosition((int)Text().Length());
					}

				}


			}
			virtual void OnKeyDown() override {
				if (!HasFocus()) return;

				// Accept keyboard input.
				if (Keyboard::LastChar()) {

					// Get the character.
					int32_t c = Keyboard::LastChar();

					// Check for character restrictions.
					if ((c == 0x09 && !__accepts_tab) || (__numeric_only && !StringUtils::IsNumeric(c)))
						Keyboard::ClearLastChar();

					else {

						// Convert the character to the appropriate case.
						switch (__character_casing) {
						case StringUtils::CharacterCasing::Lower:
							c = towlower(c); break;
						case StringUtils::CharacterCasing::Upper:
							c = towupper(c); break;
						}

						// Remove the selected text.
						RemoveSelectedText();

						// Insert the new character, and adjust the position of the caret.
						RefText().Insert(__caret.Position(), c);
						__caret.MoveRight();
						Keyboard::ClearLastChar();

					}

				}

				// Increment the key hold timer.
				bool timer_complete = false;
				if (__hold_timer < SecondsToFrames(60.0f, 0.5f))
					++__hold_timer;
				else
					timer_complete = true;
				//timer_complete = true; //debug

				// Generate keypress boolean expressions.
				bool left = ((timer_complete && Keyboard::LastKey() == ALLEGRO_KEY_LEFT && Keyboard::KeyDown(ALLEGRO_KEY_LEFT)) || Keyboard::KeyPressed(ALLEGRO_KEY_LEFT));
				bool right = ((timer_complete && Keyboard::LastKey() == ALLEGRO_KEY_RIGHT && Keyboard::KeyDown(ALLEGRO_KEY_RIGHT)) || Keyboard::KeyPressed(ALLEGRO_KEY_RIGHT));
				bool backspace = (timer_complete && Keyboard::LastKey() == ALLEGRO_KEY_BACKSPACE && Keyboard::KeyDown(ALLEGRO_KEY_BACKSPACE)) || Keyboard::KeyPressed(ALLEGRO_KEY_BACKSPACE);
				bool del = (timer_complete && Keyboard::LastKey() == ALLEGRO_KEY_DELETE && Keyboard::KeyDown(ALLEGRO_KEY_DELETE)) || Keyboard::KeyPressed(ALLEGRO_KEY_DELETE);
				bool shift = (timer_complete && (Keyboard::LastKey() == ALLEGRO_KEY_LSHIFT || Keyboard::LastKey() == ALLEGRO_KEY_RSHIFT) && Keyboard::KeyDown(KEY_SHIFT)) || Keyboard::KeyPressed(KEY_SHIFT);
				bool paste = (((timer_complete && Keyboard::LastKey() == ALLEGRO_KEY_V) && Keyboard::KeyDown(ALLEGRO_KEY_V)) || Keyboard::KeyPressed(ALLEGRO_KEY_V)) && Keyboard::KeyDown(KEY_CONTROL);

				// Trigger key events.
				if (left) {

					// Move Caret To Previous Word (Ctrl + Left)
					if (Keyboard::KeyDown(KEY_CONTROL))
						__caret.SetPosition(PrevWordPosition());

					// Move Caret Left (Left)
					else
						__caret.MoveLeft();

					// Expand Selected Region
					if (Keyboard::KeyDown(KEY_SHIFT)) {
						__sel.AssignToMax((__caret.Position()));
						__sel_draw_pos.AssignToMax(__caret.X);
					}
					else
						DeselectAll();

				}
				else if (right) {

					// Move Caret To Previous Word (Ctrl + Left)
					if (Keyboard::KeyDown(KEY_CONTROL))
						__caret.SetPosition(NextWordPosition());

					// Move Caret Right (Right)
					else
						__caret.MoveRight();

					// Expand Selected Region 
					if (Keyboard::KeyDown(KEY_SHIFT)) {
						__sel.AssignToMax((__caret.Position()));
						__sel_draw_pos.AssignToMax(__caret.X);
					}
					else
						DeselectAll();

				}
				else if (backspace) {

					// Remove The Previous Word (Ctrl + Backspace)
					if (SelectionLength() == 0 && Keyboard::KeyDown(KEY_CONTROL)) {
						int pos = PrevWordPosition();
						RefText().Remove(pos, __caret.Position() - pos);
						__caret.SetPosition(pos);
						ScrollToCaret();
					}

					// Remove the selection/previous character.
					else {
						if (SelectionLength() > 0)
							RemoveSelectedText();
						else {
							RefText().Remove(__caret.Position() - 1);
							__caret.MoveLeft();
						}
					}

				}
				else if (del) {

					// Remove The Next Word (Ctrl + Del)
					if (SelectionLength() == 0 && Keyboard::KeyDown(KEY_CONTROL)) {
						RefText().Remove(__caret.Position(), NextWordPosition() - __caret.Position());
						ScrollToCaret();
					}

					// Remove the selection/next character.
					else {
						if (SelectionLength() > 0)
							RemoveSelectedText();
						else {
							RefText().Remove(__caret.Position());
							ScrollToCaret();
						}
					}

				}
				else if (shift) {

					// If nothing is selected, reset the selection bounds.
					if (SelectionLength() == 0) DeselectAll();

				}
				else if (paste) {

					// Remove selected text.
					RemoveSelectedText();

					// Get string from the clipboard.
					String text = Clipboard::GetText();

					// Convert the string to the appropriate case.
					switch (__character_casing) {
					case StringUtils::CharacterCasing::Lower:
						text.ToLower(); break;
					case StringUtils::CharacterCasing::Upper:
						text.ToUpper(); break;
					}

					// Insert the string.
					RefText().Insert(__caret.Position(), text);
					__caret.SetPosition(__caret.Position() + (int)text.Length());

				}

				// Invalidate the Control.
				Invalidate();

			}
			virtual void OnMouseUp() override {

				// Stop allowing mouse selection.
				if (Mouse::ButtonReleased(MB_LEFT)) {
					__mouse_selecting = false;
					__full_word_selecting = false;
				}


			}
			virtual void OnMouseDown() override {

				// Focus the Control.
				Focus();

				// Set the cursor position according to where the mouse clicked.
				if (Mouse::ButtonPressed(MB_LEFT)) {
					__mouse_last_pos = Mouse::Position();
					__caret.SetPosition(GetCharacterIndexFromPoint(Mouse::Position()));
					DeselectAll();
					__mouse_selecting = true;
				}

			}
			virtual void OnMouseEnter() override {

				Mouse::SetCursor(SystemCursor::Edit);

				std::cout << "Textbox: OnMouseEnter\n";

			}
			virtual void OnMouseLeave() override {

				Mouse::SetCursor(SystemCursor::Default);

			}
			virtual void OnClick() override {

				std::cout << "\n";

				// Select the double-clicked word.


			}
			virtual void OnDoubleClick() override {

				std::cout << "Double-clicked!\n";

				// Select the double-clicked word and hide the caret.
				SelectWordAtCaret();

				// Enable full-word selection (when dragging).
				__full_word_selecting = true;

			}
			virtual void OnGotFocus() override {

				Keyboard::ClearLastChar();
				__caret.Show();
				/*__text.Append(0x30FF);
				__text.Append(0x30B3);
				__text.Append(0x3053);*/

			}
			virtual void OnLostFocus() override {

				__caret.Hide();
				//DeselectAll();

			}

			virtual void OnPaint(PaintEventArgs& e) override {

				// Draw background.
				e.Graphics().Clear(Color::Transparent);
				e.Graphics().DrawFilledRoundRectangle(0, 0, Width() - 1.0f, Height() - 1.0f, Color::White, DEF_BORDER_RADIUS);

				// Calculate text coordinates.
				Point text_pos(__scroll_h_offset + __padding, __padding);

				// Draw selection box.
				if (!__sel.IsEmpty())
					e.Graphics().DrawFilledRectangle(Rectangle(text_pos.X() + __sel_draw_pos.Start(), text_pos.Y(),
						__sel_draw_pos.Length() * (__sel_draw_pos.IsAscending() ? 1 : -1), Font()->Height()), HasFocus() ? __highlight_color : Color::Silver);

				// Draw text.
				if (Font() && !String::IsNullOrEmpty(Text()))
					e.Graphics().DrawText(text_pos.X(), text_pos.Y(), Text(), *Font(), Color::Black);
				if (__sel.Length() > 0)
					e.Graphics().DrawText(text_pos.X() + __sel_draw_pos.Min(), text_pos.Y(), Text().RefSubString(__sel.Min(), __sel.Max()), *Font(),
						HasFocus() ? Color::White : Color::DimGrey);

				// Draw cursor (if focused).
				if (HasFocus() && __caret.Visible) {
					//float cursor_x = __text.Substring(0, __caret_pos).Width(__font);
					//Graphics::SetClippingRegion(0, 0, 20, 20);
					//Graphics::DrawClear(Color::Red());
					//Graphics::ResetClippingRegion();
					//Graphics::DrawLine(Line(__caret_x, Y + 4, __caret_x, Y + Height() - 4), Color::Black(), 1);
					__caret.Draw(e);
				}

				// Draw outline.
				e.Graphics().DrawRoundRectangle(0, 0, Width() - 1.0f, Height() - 1.0f, HasFocus() ? Color::DodgerBlue : Color(17, 17, 17), DEF_BORDER_RADIUS, 1.0f);
				/*if (!__sel.IsEmpty())
					e.Graphics().Clear(Color::Blue);*/
			}
			virtual void Update(UpdateEventArgs& e) override {
				if (!HasFocus()) return;

				// Update caret (blink animation).
				__caret.Update();

				// Handle mouse dragging.
				if (Mouse::ButtonDown(MB_LEFT) && __mouse_selecting) {

					// Get Control bounds.
					Rectangle bounds = Bounds();

					// Set the caret and adjust the selection range.
					Point p = Mouse::Position();
					if (__mouse_last_pos != Mouse::Position() && Range<float>(bounds.X(), bounds.X2()).ContainsValue(Mouse::X)) {
						p.SetX(Clamp(p.X(), bounds.X() + 1, bounds.X2() - 1));
						p.SetY(bounds.Y());
						__caret.SetPosition(GetCharacterIndexFromPoint(p));
						__sel.AssignToMax((__caret.Position()));
						__sel_draw_pos.AssignToMax(__caret.X);
						__caret.Hide();
					}
					else {
						// The mouse is outside the horizontal bounds of the textbox.
						if (p.X() > bounds.X2() && __caret.Position() < Text().Length()) {
							// If the mouse is to the right of the textbox, keep increasing the selected area right.
							int speed = (std::ceil)((p.X() - bounds.X2()) / 10);
							__caret.SetPosition(__caret.Position() + speed);
							__sel.AssignToMax((__caret.Position()));
							__sel_draw_pos.AssignToMax(__caret.X);
							__caret.Hide();
						}
						else if (p.X() < bounds.X() && __caret.Position() > 0) {
							// If the mouse is to the left of the textbox, keep increasing the selected area left.
							int speed = (std::ceil)((bounds.X() - p.X()) / 10);
							__caret.SetPosition(__caret.Position() - speed);
							__sel.AssignToMax((__caret.Position()));
							__sel_draw_pos.AssignToMax(__caret.X);
							__caret.Hide();
						}
					}

					// Update our last known mouse position.
					__mouse_last_pos = Mouse::Position();

				}

			}

		private:
			class Caret : public IDrawable {
			private:
				int __blink_timer;
				int __position;
				bool __hidden;
				Textbox* __tb;
			public:
				Caret(Textbox* textbox) {
					__tb = textbox;
					__blink_timer = 0;
					__position = 0;
					__hidden = false;
					XOffset = 0;
					YOffset = 0;
					X = 0;
					Y = 0;
					Visible = false;
				}
				float XOffset, YOffset;
				float X, Y;
				bool Visible;
				void Update() {
					if (__hidden) return;

					if (__blink_timer >= 30) {
						Visible = !Visible;
						__tb->Invalidate();
						__blink_timer = 0;
					}
					else
						++__blink_timer;

				}
				void Draw(DrawEventArgs& e) override {

					float caret_x = X + XOffset + __tb->__padding + __tb->__scroll_h_offset;
					float caret_y = __tb->__padding;

					e.Graphics().DrawLine(Line(caret_x, caret_y, caret_x, caret_y + __tb->Font()->Height()), Color::Black, 1);

				}
				int Position() {

					return __position;

				}
				void MoveLeft() {

					if (__position <= 0) return;

					--__position;
					SetPosition(__position);

				}
				void MoveRight() {

					if (__position >= (int)__tb->Text().Length()) return;

					++__position;
					SetPosition(__position);

				}
				void SetPosition(int pos) {

					// Set the caret position (index).
					pos = Clamp((unsigned int)pos, 0U, (unsigned int)__tb->Text().Length());
					__position = pos;

					// Set the caret's actual position (pixel offset).
					X = __tb->Text().RefSubString(0, __position).Width(__tb->Font());

					Show();

					__tb->ScrollToCaret();

				}
				void Show() {

					Visible = true;
					__hidden = false;
					__blink_timer = 0;
					__tb->Invalidate();

				}
				void Hide() {

					Visible = false;
					__hidden = true;

					__tb->Invalidate();

				}
			};

			Caret __caret;
			Color __highlight_color;
			Range<int> __sel;
			Range<float> __sel_draw_pos;

			int __scroll_h_offset;
			int __scroll_v_offset;

			float __padding;

			float __hold_timer;
			bool __mouse_selecting, __full_word_selecting;
			Point __mouse_last_pos;
			int __last_key;

			bool __accepts_tab;
			bool __numeric_only;
			bool __use_password_char;
			int __password_char;
			StringUtils::CharacterCasing __character_casing;

			void RemoveSelectedText() {
				if (SelectionLength() <= 0) return;

				RefText().Remove(SelectionStart(), SelectionLength());
				__caret.SetPosition(SelectionStart());
				DeselectAll();

			}
			void SelectWordAtCaret() {

				// Find the left-most word boundary.
				int left = Text().LastIndexOfAny(StringUtils::IsWordBoundary, __caret.Position()) + 1;
				if (left < 0) left = 0;

				// Find the right-most word boundary.
				int right = Text().IndexOfAny(StringUtils::IsWordBoundary, __caret.Position());
				if (right < 0) right = (int)Text().Length();

				// Select the word.
				Select(left, right);

			}
			void ScrollTo(int pos) {

				// Clamp the offset within an acceptable range.
				__scroll_h_offset = Clamp((float)__scroll_h_offset, (std::min)(0.0f, -(Text().Width(*Font()) + __padding * 2 - Width())), 0.0f);

				// Calculate the visible region of text and the (actual) caret position.
				Range<float> visible_region(-__scroll_h_offset, -__scroll_h_offset + Width(), false);
				float pos_x = Text().RefSubString(pos).Width(*Font()) + __padding;

				// If the caret is already within this range, do nothing.
				if (visible_region.ContainsValue(pos_x)) return;

				if ((Keyboard::LastChar() && !(pos == Text().Length()))) {
					// If a key was pressed and the cursor is not at the end of the textbox, scroll the textbox such that the caret is in the middle of the textbox (or at the end).
					int diff_to_end = (Text().Width(*Font()) + __padding * 2 - Width()) + __scroll_h_offset;
					__scroll_h_offset -= (std::min)(visible_region.Length() / 2, diff_to_end);

				}
				else if (Keyboard::KeyDown(ALLEGRO_KEY_BACKSPACE)) {
					// Function similarily to when a key is pressed, except moving left instead of right.
					__scroll_h_offset += (std::min)(visible_region.Length() / 2, -__scroll_h_offset);
				}
				else {
					// If the caret is to the right the visible region, move region right.
					if (pos_x >= visible_region.End())
						__scroll_h_offset -= (pos_x - visible_region.End()) + __padding;

					// If the caret is to the left of the visible region, move region left.
					else if (pos_x <= visible_region.Start())
						__scroll_h_offset += (visible_region.Start() - pos_x) + __padding;
					// If pasting in front of text, scroll the textbox such that the caret is in the middle of the textbox (or at the end).
					if (Keyboard::KeyDown(ALLEGRO_KEY_LCTRL) && Keyboard::KeyDown(ALLEGRO_KEY_V) && !(pos == Text().Length())) {
						int diff_to_end = (Text().Width(*Font()) + __padding * 2 - Width()) + __scroll_h_offset;
						__scroll_h_offset -= (std::min)(visible_region.Length() / 2, diff_to_end);
					}
				}

			}
			int NextWordPosition() {

				// Find the index of the next word boundary character.
				int wb_pos = Text().IndexOfAny(StringUtils::IsWordBoundary, __caret.Position());

				// If no boundary was found, go to the end of the string.
				if (wb_pos == -1) return (int)Text().Length();

				// Move forward until we're no longer sitting on a word boundary.
				while (wb_pos < Text().Length() && StringUtils::IsWordBoundary(Text().CharAt(wb_pos)))
					++wb_pos;

				// Return the result.
				return wb_pos;

			}
			int PrevWordPosition() {

				// Find the index of the previous word boundary character.
				int wb_pos = Text().LastIndexOfAny(StringUtils::IsWordBoundary, __caret.Position());

				// Move backward until we're no longer sitting on a word boundary.
				while (StringUtils::IsWordBoundary(Text().CharAt(wb_pos - 1)))
					--wb_pos;

				// Get the index of the next word boundary (the start of the current word).
				wb_pos = Text().LastIndexOfAny(StringUtils::IsWordBoundary, wb_pos) + 1;

				// Return the result.
				return wb_pos;

			}

		};

	}

}