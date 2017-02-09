#ifndef __GUITEXTBOX_H
#define __GUITEXTBOX_H
#include <algorithm>
#include "Control.h"
#include "Range.h"
#include "String.h"
#include "Clipboard.h"

/*
Todo:
- Word-by-word drag selection
- Implement undo/redo buffer
*/

namespace GUI {
	class Textbox : public Control {
		friend class Caret;

	private:
		Utf8String __text;
		Font* __font;

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
			void Draw() override {
				float caret_x = X + XOffset + __tb->__padding + __tb->__scroll_h_offset;
				float caret_y = __tb->Y + __tb->__padding;
				Graphics::DrawLine(Line(caret_x, caret_y, caret_x, caret_y + __tb->__font->Height()), Color::Black(), 1);
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

				if (__position >= (int)__tb->__text.Length()) return;

				++__position;
				SetPosition(__position);

			}
			void SetPosition(int pos) {

				// Set the caret position (index).
				pos = Clamp(pos, 0, __tb->__text.Length());
				__position = pos;

				// Set the caret's actual position (pixel offset).
				X = __tb->__text.RefSubstring(0, __position).Width(__tb->__font);

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
		CharacterCasing __character_casing;

		void RemoveSelectedText() {
			if (SelectionLength() <= 0) return;

			__text.Remove(SelectionStart(), SelectionLength());
			__caret.SetPosition(SelectionStart());
			DeselectAll();

		}
		void SelectWordAtCaret() {

			// Find the left-most word boundary.
			int left = __text.LastIndexOfAny(String::IsWordBoundary, __caret.Position()) + 1;
			if (left < 0) left = 0;

			// Find the right-most word boundary.
			int right = __text.IndexOfAny(String::IsWordBoundary, __caret.Position());
			if (right < 0) right = (int)__text.Length();

			// Select the word.
			Select(left, right);

		}
		void ScrollTo(int pos) {

			// Clamp the offset within an acceptable range.
			__scroll_h_offset = Clamp(__scroll_h_offset, (std::min)(0.0f, -(__text.Width(__font) + __padding * 2 - Width())), 0);

			// Calculate the visible region of text and the (actual) caret position.
			Range<float> visible_region(-__scroll_h_offset, -__scroll_h_offset + Width(), false);
			float pos_x = __text.RefSubstring(pos).Width(__font) + __padding;

			// If the caret is already within this range, do nothing.
			if (visible_region.ContainsValue(pos_x)) return;

			if ((Keyboard::LastChar() && !(pos == __text.Length()))) {
				// If a key was pressed and the cursor is not at the end of the textbox, scroll the textbox such that the caret is in the middle of the textbox (or at the end).
				int diff_to_end = (__text.Width(__font) + __padding * 2 - Width()) + __scroll_h_offset;
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
				if (Keyboard::KeyDown(ALLEGRO_KEY_LCTRL) && Keyboard::KeyDown(ALLEGRO_KEY_V) && !(pos == __text.Length())) {
					int diff_to_end = (__text.Width(__font) + __padding * 2 - Width()) + __scroll_h_offset;
					__scroll_h_offset -= (std::min)(visible_region.Length() / 2, diff_to_end);
				}
			}

		}
		int NextWordPosition() {

			// Find the index of the next word boundary character.
			int wb_pos = __text.IndexOfAny(String::IsWordBoundary, __caret.Position());

			// If no boundary was found, go to the end of the string.
			if (wb_pos == -1) return (int)__text.Length();

			// Move forward until we're no longer sitting on a word boundary.
			while (wb_pos < __text.Length() && String::IsWordBoundary(__text.CharAt(wb_pos)))
				++wb_pos;

			// Return the result.
			return wb_pos;

		}
		int PrevWordPosition() {

			// Find the index of the previous word boundary character.
			int wb_pos = __text.LastIndexOfAny(String::IsWordBoundary, __caret.Position());

			// Move backward until we're no longer sitting on a word boundary.
			while (String::IsWordBoundary(__text.CharAt(wb_pos - 1)))
				--wb_pos;

			// Get the index of the next word boundary (the start of the current word).
			wb_pos = __text.LastIndexOfAny(String::IsWordBoundary, wb_pos) + 1;

			// Return the result.
			return wb_pos;

		}

	public:
		Textbox(float x, float y, float width, float height) : Control(Point(x, y), Size(width, height)),
			__caret(this),
			__sel(0, 0),
			__sel_draw_pos(0.0f, 0.0f),
			__mouse_last_pos(Mouse::X, Mouse::Y) {

			__font = GUI::ActiveTheme()->DefaultFont();
			__highlight_color = GUI::ActiveTheme()->HighlightColor();
			__text = "";
			__padding = 3.0f;
			__hold_timer = 0.0f;
			__mouse_selecting = false;
			__full_word_selecting = false;

			__character_casing = CharacterCasing::Default;
			__accepts_tab = false;
			__numeric_only = false;
			__use_password_char = false;
			__password_char = '*';

		}

		Utf8String Text() {

			return __text;

		}
		Utf8String SelectedText() {

			return __text.Substring(__sel.Min(), __sel.Max());

		}

		void SetText(const char* text) {

			__text = text;

			Invalidate();

		}
		void AppendText(const char* text) {

			__text.Append(text);
			Invalidate();

		}
		void AppendText(int ch) {

			__text.Append(ch);
			Invalidate();

		}

		void Select(int start, int end) {

			// Set the selection area.
			__sel.Resize(start, end);

			// Set the selection bounds.
			int start_x = __text.RefSubstring(0, start).Width(__font);
			int end_x = start_x + __text.RefSubstring(start, end).Width(__font);
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
			Select(0, (int)__text.Length());

		}
		void DeselectAll() {

			__sel.Resize(__caret.Position(), __caret.Position());
			__sel_draw_pos.Resize(__caret.X(), __caret.X);

		}

		void ScrollToCaret() {

			// Clamp the offset within an acceptable range.
			__scroll_h_offset = Clamp(__scroll_h_offset, (std::min)(0.0f, -(__text.Width(__font) + __padding * 2 - Width())), 0);

			// Calculate the visible region of text and the (actual) caret position.
			Range<float> visible_region(-__scroll_h_offset, -__scroll_h_offset + Width(), false);
			float caret_x = __caret.X() + __padding;

			// If the caret is already within this range, do nothing.
			if (visible_region.ContainsValue(caret_x)) return;

			if ((Keyboard::LastChar() && !(__caret.Position() == __text.Length()))) {
				// If a key was pressed and the cursor is not at the end of the textbox, scroll the textbox such that the caret is in the middle of the textbox (or at the end).
				int diff_to_end = (__text.Width(__font) + __padding * 2 - Width()) + __scroll_h_offset;
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
				if (Keyboard::KeyDown(ALLEGRO_KEY_LCTRL) && Keyboard::KeyDown(ALLEGRO_KEY_V) && !(__caret.Position() == __text.Length())) {
					int diff_to_end = (__text.Width(__font) + __padding * 2 - Width()) + __scroll_h_offset;
					__scroll_h_offset -= (std::min)(visible_region.Length() / 2, diff_to_end);
				}
			}

		}

		int GetCharacterIndexFromPoint(Point point) {

			// Initialize variables that we'll need.
			static const int ERROR_THRESHOLD = 3;
			Point real = FixedPosition();
			int width = __text.Width(__font);
			int text_x1 = real.X() + __scroll_h_offset + __padding;
			int text_x2 = text_x1 + width;

			// If the string is empty, return first index.
			if (Utf8String::IsNullOrEmpty(__text)) return 0;

			// If the point is out of the textbox bounds, return first index.
			if (!PointIn(point, Rectangle(real.X(), real.Y(), Width(), Height()))) return 0;

			// If the point is beyond the position of the string, return the last index.
			if (point.X() > text_x2) return (int)__text.Length();

			// To select the best starting position, figure out what percentage down the string width our point is.
			double percent = (point.X() - text_x1) / width;
			int pos = (std::round)(__text.Length() * percent);
			int pos_x = text_x1 + __text.RefSubstring(0, pos).Width(__font);
			int err = point.X() - pos_x;
			int aerr = (std::abs)(err);

			// If the error is above the threshold, keep attempting to correct it until it can no longer be improved.
			if (aerr >= ERROR_THRESHOLD) {
				int besterr = aerr;
				int new_pos, new_pos_x, new_err;
				do {
					new_pos = pos + Sign(err);
					new_pos_x = text_x1 + __text.RefSubstring(0, new_pos).Width(__font);
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
		CharacterCasing CharacterCasing() {

			return __character_casing;

		}
		void  SetCharacterCasing(::CharacterCasing value) {

			// Convert the text into the new casing (if applicable).
			switch (value) {
			case ::CharacterCasing::Lower:
				__text.ToLower(); break;
			case ::CharacterCasing::Upper:
				__text.ToUpper(); break;
			}

			__character_casing = value;

		}
		float Padding() {

			return __padding;

		}
		void SetPadding(float value) {

			__padding = value;

		}
		Font* Font() {

			return __font;

		}
		void SetFont(::Font* value) {

			__font = value;

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
					__caret.SetPosition((int)__text.Length());
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
					Select(__sel.Start(), (int)__text.Length());
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
					__caret.SetPosition((int)__text.Length());
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
				if ((c == 0x09 && !__accepts_tab) || (__numeric_only && !String::IsNumeric(c)))
					Keyboard::ClearLastChar();

				else {

					// Convert the character to the appropriate case.
					switch (__character_casing) {
					case ::CharacterCasing::Lower:
						c = towlower(c); break;
					case ::CharacterCasing::Upper:
						c = towupper(c); break;
					}

					// Remove the selected text.
					RemoveSelectedText();

					// Insert the new character, and adjust the position of the caret.
					__text.Insert(__caret.Position(), c);
					__caret.MoveRight();
					Keyboard::ClearLastChar();

				}

			}

			// Increment the key hold timer.
			bool timer_complete = false;
			if (__hold_timer < SecondsToFrames(0.5f))
				++__hold_timer;
			else
				timer_complete = true;

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
					__text.Remove(pos, __caret.Position() - pos);
					__caret.SetPosition(pos);
					ScrollToCaret();
				}

				// Remove the selection/previous character.
				else {
					if (SelectionLength() > 0)
						RemoveSelectedText();
					else {
						__text.Remove(__caret.Position() - 1);
						__caret.MoveLeft();
					}
				}

			}
			else if (del) {

				// Remove The Next Word (Ctrl + Del)
				if (SelectionLength() == 0 && Keyboard::KeyDown(KEY_CONTROL)) {
					__text.Remove(__caret.Position(), NextWordPosition() - __caret.Position());
					ScrollToCaret();
				}

				// Remove the selection/next character.
				else {
					if (SelectionLength() > 0)
						RemoveSelectedText();
					else {
						__text.Remove(__caret.Position());
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
				Utf8String text = Clipboard::GetText();

				// Convert the string to the appropriate case.
				switch (__character_casing) {
				case ::CharacterCasing::Lower:
					text.ToLower(); break;
				case ::CharacterCasing::Upper:
					text.ToUpper(); break;
				}

				// Insert the string.
				__text.Insert(__caret.Position(), text);
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
			DeselectAll();

		}

		virtual void OnPaint() override {

			// Draw background.
			Graphics::DrawClear(Color::Transparent());
			Graphics::DrawFilledRoundRect(Rectangle(X, Y, Width() - 1.0f, Height() - 1.0f), 3.0f, Color::White());

			// Calculate text coordinates.
			Point text_pos(X + __scroll_h_offset + __padding, Y + __padding);

			// Draw selection box.
			if (!__sel.IsEmpty())
				Graphics::DrawFilledRectangle(Rectangle(text_pos.X() + __sel_draw_pos.Start(), text_pos.Y(),
					__sel_draw_pos.Length() * (__sel_draw_pos.IsAscending() ? 1 : -1), __font->Height()), HasFocus() ? __highlight_color : Color::Silver());

			// Draw text.
			if (__font && !Utf8String::IsNullOrEmpty(__text))
				Graphics::DrawText(text_pos.X(), text_pos.Y(), __text, __font, Color::Black());
			if (__sel.Length() > 0)
				Graphics::DrawText(text_pos.X() + __sel_draw_pos.Min(), text_pos.Y(), __text.RefSubstring(__sel.Min(), __sel.Max()), __font,
					HasFocus() ? Color::White() : Color::DimGrey());

			// Draw cursor (if focused).
			if (HasFocus() && __caret.Visible) {
				//float cursor_x = __text.Substring(0, __caret_pos).Width(__font);
				//Graphics::SetClippingRegion(0, 0, 20, 20);
				//Graphics::DrawClear(Color::Red());
				//Graphics::ResetClippingRegion();
				//Graphics::DrawLine(Line(__caret_x, Y + 4, __caret_x, Y + Height() - 4), Color::Black(), 1);
				__caret.Draw();
			}

			// Draw outline.
			Graphics::DrawRoundRect(Rectangle(X, Y, Width() - 1.0f, Height() - 1.0f), 3.0f, HasFocus() ? Color::DodgerBlue() : Color(17, 17, 17), 1.0f);

		}

		virtual void Update() override {
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
					p.X() = Clamp(p.X(), bounds.X() + 1, bounds.X2() - 1);
					p.Y() = bounds.Y;
					__caret.SetPosition(GetCharacterIndexFromPoint(p));
					__sel.AssignToMax((__caret.Position()));
					__sel_draw_pos.AssignToMax(__caret.X);
					__caret.Hide();
				}
				else {
					// The mouse is outside the horizontal bounds of the textbox.
					if (p.X() > bounds.X2() && __caret.Position() < __text.Length()) {
						// If the mouse is to the right of the textbox, keep increasing the selected area right.
						int speed = (std::ceil)((p.X() - bounds.X2()) / 10);
						__caret.SetPosition(__caret.Position() + speed);
						__sel.AssignToMax((__caret.Position()));
						__sel_draw_pos.AssignToMax(__caret.X);
						__caret.Hide();
					}
					else if (p.X() < bounds.X() && __caret.Position() > 0) {
						// If the mouse is to the left of the textbox, keep increasing the selected area left.
						int speed = (std::ceil)((bounds.X() - p.X) / 10);
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

	};
}

#endif