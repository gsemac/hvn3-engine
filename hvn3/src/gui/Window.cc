#include "gui/Window.h"
#include "gui/ControlManager.h"
#include "core/UpdateEventArgs.h"
#define RESIZE_REGION_WIDTH 4.0f
#define DEF_TITLEBAR_HEIGHT 29.0f
#define DEF_OUTLINE_WIDTH 1.0f

namespace hvn3 {

	namespace Gui {

		Window::Window(float x, float y, float width, float height, const char* text) :
			Control(PointF(x, y), SizeF(width + DEF_OUTLINE_WIDTH * 2, height + DEF_TITLEBAR_HEIGHT)),
			ITextable(this, text),
			IContainer(this),
			//_panel(DEF_OUTLINE_WIDTH, DEF_TITLEBAR_HEIGHT, width - DEF_OUTLINE_WIDTH, height - DEF_OUTLINE_WIDTH),
			_drag_offset(0.0f, 0.0f),
			_original_position(x, y),
			_original_size(width + DEF_OUTLINE_WIDTH * 2, height + DEF_TITLEBAR_HEIGHT),
			_size_diff(0, 0),
			_exit_icon(nullptr) {

			// Make sure the Window cannot be made smaller than its titlebar.
			SetMinimumSize(SizeF(DEF_TITLEBAR_HEIGHT, DEF_TITLEBAR_HEIGHT));

			// Set up state/state tracking variables.
			_dragging = false;
			_resizing = false;
			_resizing_side = 0;
			_titlebar_height = DEF_TITLEBAR_HEIGHT;
			_mouse_on_exit_button = false;
			_fade_out = false;

			UpdateChildRegion();

			UpdateDockableRegion();

			// Set the control offset for the control manager to account for the header bar.
			Controls()->SetControlOffset(Controls()->ControlOffset().X() + DEF_OUTLINE_WIDTH, _titlebar_height);

		}
		Window::~Window() {

		}

		void Window::SetTitlebarHeight(float value) {

			_titlebar_height = value;

			Invalidate();

		}
		float Window::TitlebarHeight() const {

			return _titlebar_height;

		}

		void Window::OnMouseDown(MouseEventArgs& e) {

			// Place the Window in front of all other Controls under the same manager.
			if (!MouseOnExitButton())
				BringToFront();

			if (HasActiveChild())
				return;

			if (!_dragging && !_resizing && GetMouseResizeSides()) {

				// Initialize resizing variables.
				_resizing = true;
				_resizing_side = GetMouseResizeSides();
				_drag_offset = PointF(Mouse::X, Mouse::Y);
				_original_size = SizeF(Width(), Height());
				_original_position = PointF(X(), Y());
				_size_diff = SizeF(0, 0);

			}
			else if (!_resizing && !_dragging) {

				// Initialize dragging variables.
				_dragging = true;
				_drag_offset = PointF(X() - Mouse::X, Y() - Mouse::Y);

			}

		}
		void Window::OnMouseUp(MouseEventArgs& e) {
			if (HasActiveChild()) return;

			// Stop dragging/resizing.
			_dragging = false;
			_resizing = false;

			// Reset the cursor to the default.
			Mouse::SetCursor(SystemCursor::Default);

		}
		void Window::OnMouseMove(MouseMoveEventArgs& e) {

			if (HasActiveChild())
				return;

			// Set the cursor to a resize cursor if it passes over any edges.
			if (!_resizing && !_dragging)
				SetResizeCursor();

			// Invalid the window when the mouse hovers over any dynamic elements.
			if (MouseOnExitButton() != _mouse_on_exit_button) {
				_mouse_on_exit_button = MouseOnExitButton();
				Invalidate();
			}

		}
		void Window::OnMouseLeave() {

			// If the mouse leaves the Control and it's not resizing, reset the cursor.
			if (!_resizing) Mouse::SetCursor(SystemCursor::Default);

		}
		void Window::OnClick() {

			if (MouseOnExitButton())
				_fade_out = true;

		}
		void Window::OnResize(ResizeEventArgs& e) {

			// Set child region (region in which child controls can be interacted with).
			UpdateChildRegion();

			UpdateAnchors(e);

			UpdateDockableRegion();

		}
		void Window::OnPaint(PaintEventArgs& e) {

			// Draw main window area.
			e.Graphics().DrawFilledRectangle(0, TitlebarHeight() - DEF_OUTLINE_WIDTH, Width(), Height() - TitlebarHeight(), BackColor());

			// Draw Panel containing the child Controls.
			Controls()->InvalidateAll();
			Controls()->OnDraw(e);

			// Draw main window border.
			e.Graphics().DrawRectangle(0, TitlebarHeight() - 1.0f, Width(), Height() - TitlebarHeight() + 1.0f, Color(17, 17, 17), 1);

			// Draw titlebar.
			e.Graphics().DrawFilledRectangle(0, 0, Width(), TitlebarHeight(), BackColor());
			e.Graphics().DrawRectangle(0, 0, Width(), TitlebarHeight(), Color(17, 17, 17), 1);

			// Draw titlebar text.
			if (Font()) {
				float tx = Math::Round(Width() / 2.0f);
				float ty = Math::Round((TitlebarHeight() / 2.0f) - (Font()->Height() / 2.0f)) - 1;
				e.Graphics().DrawText(tx + 1, ty + 1, Text(), Font(), Color(0, 0, 0, 128), Alignment::Center);
				e.Graphics().DrawText(tx, ty, Text(), Font(), Color(186, 186, 186), Alignment::Center);
			}

			// Draw exit button.
			if (GetExitIcon()) {
				PointF pos = ExitButtonPosition();
				Color tint = _mouse_on_exit_button ? Color::FromArgbf(0.5f, 0.5f, 0.5f, 1.0f) : Color::FromArgbf(1.0f, 1.0f, 1.0f, 1.0f);
				e.Graphics().DrawBitmap(pos.X(), pos.Y(), _exit_icon, tint);
			}

		}
		void Window::OnUpdate(UpdateEventArgs& e) {

			if (IsActiveControl() && MouseInChildRegion()) {
				Controls()->SetMouseEventsEnabled(true);
			}
			else {
				Controls()->SetMouseEventsEnabled(false);
				Controls()->ClearActiveControl();
			}

			Controls()->OnUpdate(e);

			if (!Mouse::ButtonDown(MouseButton::Left) && (_dragging || _resizing)) {

				_dragging = false;
				_resizing = false;
				Mouse::SetCursor(SystemCursor::Default);

			}

			if (_resizing)
				HandleResizing();
			else if (_dragging) {
				SetPosition(_drag_offset.X() + Mouse::X, _drag_offset.Y() + Mouse::Y);
				if (Parent())
					Parent()->Invalidate();
			}

			if (_fade_out) {
				if (Opacity() > 0.0f)
					SetOpacity(Opacity() - 5.0f * e.Delta());
				else
					Manager()->ControlManager()->RemoveControl(this);
				Invalidate();
			}

		}

		// Protected methods

		const ResourceHandle<Graphics::Bitmap>& Window::GetExitIcon() {

			if (!_exit_icon && Manager())
				_exit_icon = Manager()->StyleManager()->GetImageResource(Gui::BitmapResourceId::ExitButton);

			return _exit_icon;

		}
		PointF Window::ExitButtonPosition() const {

			float ex = Width() - _exit_icon->Width() - (_exit_icon->Width() / 2.0f);
			float ey = (_exit_icon->Height() / 2.0f) + 1.0f;

			return PointF(ex, ey);

		}
		bool Window::MouseOnExitButton() const {

			PointF pos = ExitButtonPosition();
			PointF my_pos = FixedPosition();

			return Mouse::InRegion(
				my_pos.X() + pos.X(),
				my_pos.Y() + pos.Y(),
				my_pos.X() + pos.X() + _exit_icon->Width(),
				my_pos.Y() + pos.Y() + _exit_icon->Height()
				);

		}
		void Window::UpdateChildRegion() {

			SetChildRegion(RectangleF(RESIZE_REGION_WIDTH, _titlebar_height, Width() - RESIZE_REGION_WIDTH * 2, Height() - _titlebar_height - RESIZE_REGION_WIDTH));

		}
		void Window::UpdateDockableRegion() {

			Controls()->SetDockableRegion(RectangleF(0.0f, 0.0f, Width() - DEF_OUTLINE_WIDTH * 2, Height() - DEF_OUTLINE_WIDTH - _titlebar_height));

		}

		// Private methods

		unsigned int Window::GetMouseResizeSides() const {

			// A combination of sides indicating which sides the mouse is hovering over (maximum of 2).
			unsigned int side = 0;
			// The control's scale factor.
			float scale = Scale();
			// The global position of the control.
			PointF gp = FixedPosition();

			float rrw = RESIZE_REGION_WIDTH;

			if (Mouse::InRegion(gp.X(), gp.Y() + (Height() - rrw) * scale, gp.X() + Width() * scale, gp.Y() + (Height() + TitlebarHeight()) * scale))
				side |= BOTTOM;

			else if (Mouse::InRegion(gp.X(), gp.Y(), gp.X() + Width() * scale, gp.Y() + rrw * scale))
				side |= TOP;

			if (Mouse::InRegion(gp.X(), gp.Y(), gp.X() + rrw * scale, gp.Y() + Height() * scale))
				side |= LEFT;

			else if (Mouse::InRegion(gp.X() + Width() * scale - rrw * scale, gp.Y(), gp.X() + Width() * scale, gp.Y() + Height() * scale))
				side |= RIGHT;

			return side;

		}
		void Window::SetResizeCursor() {

			unsigned int region = GetMouseResizeSides();

			if (!region)
				Mouse::SetCursor(SystemCursor::Default);

			if (region & TOP)
				Mouse::SetCursor(SystemCursor::ResizeN);
			else if (region & BOTTOM)
				Mouse::SetCursor(SystemCursor::ResizeS);

			if (region & LEFT)
				if (region & TOP)
					Mouse::SetCursor(SystemCursor::ResizeNW);
				else if (region & BOTTOM)
					Mouse::SetCursor(SystemCursor::ResizeSW);
				else
					Mouse::SetCursor(SystemCursor::ResizeW);

			else if (region & RIGHT)
				if (region & TOP)
					Mouse::SetCursor(SystemCursor::ResizeNE);
				else if (region & BOTTOM)
					Mouse::SetCursor(SystemCursor::ResizeSE);
				else
					Mouse::SetCursor(SystemCursor::ResizeE);

		}
		void Window::HandleResizing() {

			float new_width = Width();
			float new_height = Height();
			float scale = Scale();

			if (_resizing_side & BOTTOM) {

				// Calculate the differnce in height between the mouse's current position and it's new position.
				_size_diff.SetHeight(Mouse::Y - _drag_offset.Y());

				// If there is a difference, resize the Control.
				if (std::abs((_original_size.Height() + _size_diff.Height()) - Height()) > 0.0f)
					new_height = _original_size.Height() + (_size_diff.Height() / scale);

			}
			else if (_resizing_side & TOP) {

				// Calculate the differnce in height between the mouse's current position and it's new position.
				_size_diff.SetHeight(Mouse::Y - _drag_offset.Y());

				// If there is a difference, resize the Control.
				if (std::abs((_original_size.Height() - _size_diff.Height()) - Height()) > 0.0f) {
					new_height = Math::Clamp(_original_size.Height() - (_size_diff.Height() / scale), MinimumSize().Height(), MaximumSize().Height());
					SetY(_original_position.Y() + (_original_size.Height() - new_height) * scale);
				}

			}
			if (_resizing_side & RIGHT) {

				// Calculate the difference in width between the mouse's current position and it's new position.
				_size_diff.SetWidth(Mouse::X - _drag_offset.X());

				// If there is a difference, resize the Control.
				if (std::abs((_original_size.Width() + _size_diff.Width()) - Width()) > 0.0f)
					new_width = _original_size.Width() + (_size_diff.Width() / scale);

			}
			else if (_resizing_side & LEFT) {

				// Calculate the difference in width between the mouse's current position and it's new position.
				_size_diff.SetWidth(Mouse::X - _drag_offset.X());

				// If there is a difference, resize the Control.
				if (std::abs((_original_size.Width() - _size_diff.Width()) - Width()) > 0.0f) {
					new_width = Math::Clamp(_original_size.Width() - (_size_diff.Width() / scale), MinimumSize().Width(), MaximumSize().Width());
					SetX(_original_position.X() + (_original_size.Width() - new_width) * scale);
				}

			}

			if (new_width != Width() || new_height != Height())
				Resize(std::round(new_width), std::round(new_height));

		}
		bool Window::HasActiveChild() {

			return Controls()->ActiveControl();

		}

	}

}