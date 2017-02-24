#include "GUIWindow.h"
#define RESIZE_REGION_WIDTH 4.0f
#define DEF_TITLEBAR_HEIGHT 29.0f
#define DEF_OUTLINE_WIDTH 1.0f

namespace GUI {

	// Public

	Window::Window(float x, float y, float width, float height, const char* text) :
		Control(Point(x, y), Size(width, height + DEF_TITLEBAR_HEIGHT)),
		__panel(DEF_OUTLINE_WIDTH, DEF_TITLEBAR_HEIGHT, width - DEF_OUTLINE_WIDTH, height - DEF_OUTLINE_WIDTH),
		__drag_offset(0.0f, 0.0f),
		__original_position(x, y),
		__original_size(width, height + DEF_TITLEBAR_HEIGHT),
		__size_diff(0, 0)
	{

		// Set up titlebar.
		__font = ActiveTheme()->DefaultFont();
		__exit_icon = ActiveTheme()->ExitIcon();
		__text = new Utf8String(text);

		// Make sure the Window cannot be made smaller than its titlebar.
		SetMinimumSize(Size(DEF_TITLEBAR_HEIGHT, DEF_TITLEBAR_HEIGHT));

		// Set up Panel.
		__panel.SetParent(this);

		// Set up state/state tracking variables.
		__dragging = false;
		__resizing = false;
		__resizing_side = 0;
		__titlebar_height = DEF_TITLEBAR_HEIGHT;

	}
	Window::~Window() {

		// Delete text objext.
		if (__text) delete __text;

	}

	void Window::AddControl(Control* control) {

		// Add the Control to the Panel.
		__panel.AddControl(control);

	}
	void Window::RemoveControl(Control* control) {

		// Remove the Control from the Panel.
		__panel.RemoveControl(control);

	}
	std::list<Control*>& Window::Controls() {

		// Return a reference to the Panel's Controls.
		return __panel.Controls();

	}
	void Window::SetTitlebarHeight(float value) {

		__titlebar_height = value;

	}
	float Window::TitlebarHeight() const {

		return __titlebar_height;

	}

	void Window::OnMouseDown() {

		// Place the Window in front of all other Controls under the same manager.
		BringToFront();

		if (HasActiveChild()) return;

		if (!__dragging && !__resizing && GetMouseResizeRegions()) {

			// Initialize resizing variables.
			__resizing = true;
			__resizing_side = GetMouseResizeRegions();
			__drag_offset.SetXY(Mouse::X, Mouse::Y);
			__original_size = Size(Width(), Height());
			__original_position = Point(X(), Y());
			__size_diff = Size(0, 0);

		}
		else if (!__resizing && !__dragging) {

			// Initialize dragging variables.
			__dragging = true;
			__drag_offset.SetXY(X() - Mouse::X, Y() - Mouse::Y);

		}

	}
	void Window::OnMouseUp() {
		if (HasActiveChild()) return;

		// Stop dragging/resizing.
		__dragging = false;
		__resizing = false;

		// Reset the cursor to the default.
		Mouse::SetCursor(SystemCursor::Default);

	}
	void Window::OnMouseMove() {
		if (HasActiveChild()) return;

		// Set the cursor to a resize cursor if it passes over any edges.
		if (!__resizing && !__dragging) SetResizeCursor();

	}
	void Window::OnMouseLeave() {
		std::cout << "Window: OnMouseLeave()\n";
		// If the mouse leaves the Control and it's not resizing, reset the cursor.
		if (!__resizing) Mouse::SetCursor(SystemCursor::Default);

	}
	void Window::OnResize() {

		// Resize the Panel.
		__panel.Resize(Width(), Height() - TitlebarHeight() - DEF_OUTLINE_WIDTH);

	}
	void Window::OnPaint(PaintEventArgs& e) {

		// Draw titlebar.
		e.Graphics().DrawFilledRectangle(0, 0, Width(), TitlebarHeight(), BackColor());
		e.Graphics().DrawRectangle(0, 0, Width(), TitlebarHeight(), Color(17, 17, 17), 1);

		// Draw titlebar text.
		float tx = Round(Width() / 2.0f);
		float ty = Round((TitlebarHeight() / 2.0f) - (__font->Height() / 2.0f)) - 1;
		e.Graphics().DrawText(tx + 1, ty + 1, *__text, *__font, Color(0, 0, 0, 0.5f), Alignment::Center);
		e.Graphics().DrawText(tx, ty, *__text, *__font, Color(186, 186, 186), Alignment::Center);

		// Draw exit button.
		float ex = Width() - __exit_icon->Width() - (__exit_icon->Width() / 2.0f);
		float ey = (__exit_icon->Height() / 2.0f) + 1.0f;
		Color tint = Mouse::InRegion(X() + ex, Y() + ey, X() + ex + __exit_icon->Width(), Y() + ey + __exit_icon->Height()) ?
			Color::FromArgbf(0.5f, 0.5f, 0.5f, 1.0f) : Color::FromArgbf(1.0f, 1.0f, 1.0f, 1.0f);
		e.Graphics().DrawBitmap(ex, ey, __exit_icon->SubImage(0), tint);

		// Draw main window area.
		e.Graphics().DrawFilledRectangle(0, TitlebarHeight() - DEF_OUTLINE_WIDTH, Width(), Height() - TitlebarHeight(), BackColor());

		// Draw Panel containing the child Controls.
		__panel.Draw(e);

		// Draw main window border.
		e.Graphics().DrawRectangle(0, TitlebarHeight() - 1.0f, Width(), Height() - TitlebarHeight(), Color(17, 17, 17), 1);

	}
	void Window::Update(UpdateEventArgs& e) {

		__panel.ChildManager()->SetMouseEventsEnabled((Manager()->ActiveControl() == this));
		__panel.Update(e);

		if (!Mouse::ButtonDown(MB_LEFT) && (__dragging || __resizing)) {
			__dragging = false;
			__resizing = false;
			Mouse::SetCursor(SystemCursor::Default);

		}

		if (__resizing)
			HandleResizing();
		else if (__dragging) {
			SetXY(__drag_offset.X() + Mouse::X, __drag_offset.Y() + Mouse::Y);
		}

	}

	// Private

	unsigned int Window::GetMouseResizeRegions() {

		unsigned int side = 0;
		float scale = Scale();

		if (Mouse::InRegion(
			X(),
			Y() + (Height() - RESIZE_REGION_WIDTH) * scale,
			X() + Width() * scale,
			Y() + (Height() + TitlebarHeight()) * scale))
			side |= BOTTOM;

		else if (Mouse::InRegion(X(), Y(), X() + Width() * scale, Y() + RESIZE_REGION_WIDTH * scale))
			side |= TOP;

		if (Mouse::InRegion(X(), Y(), X() + RESIZE_REGION_WIDTH * scale, Y() + Height() * scale))
			side |= LEFT;

		else if (Mouse::InRegion(X() + Width() * scale - RESIZE_REGION_WIDTH * scale, Y(), X() + Width() * scale, Y() + Height() * scale))
			side |= RIGHT;
		return side;

	}
	void Window::SetResizeCursor() {

		unsigned int region = GetMouseResizeRegions();

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

		if (__resizing_side & BOTTOM) {

			// Calculate the differnce in height between the mouse's current position and it's new position.
			__size_diff.SetHeight(Mouse::Y - __drag_offset.Y());

			// If there is a difference, resize the Control.
			if (std::abs((__original_size.Height() + __size_diff.Height()) - Height()) > 0.0f)
				new_height = __original_size.Height() + (__size_diff.Height() / scale);

		}
		else if (__resizing_side & TOP) {

			// Calculate the differnce in height between the mouse's current position and it's new position.
			__size_diff.SetHeight(Mouse::Y - __drag_offset.Y());

			// If there is a difference, resize the Control.
			if (std::abs((__original_size.Height() - __size_diff.Height()) - Height()) > 0.0f) {
				new_height = Clamp(__original_size.Height() - (__size_diff.Height() / scale), MinimumSize().Height(), MaximumSize().Height());
				SetY(__original_position.Y() + (__original_size.Height() - new_height) * scale);
			}

		}
		if (__resizing_side & RIGHT) {

			// Calculate the difference in width between the mouse's current position and it's new position.
			__size_diff.SetWidth(Mouse::X - __drag_offset.X());

			// If there is a difference, resize the Control.
			if (std::abs((__original_size.Width() + __size_diff.Width()) - Width()) > 0.0f)
				new_width = __original_size.Width() + (__size_diff.Width() / scale);

		}
		else if (__resizing_side & LEFT) {

			// Calculate the difference in width between the mouse's current position and it's new position.
			__size_diff.SetWidth(Mouse::X - __drag_offset.X());

			// If there is a difference, resize the Control.
			if (std::abs((__original_size.Width() - __size_diff.Width()) - Width()) > 0.0f) {
				new_width = Clamp(__original_size.Width() - (__size_diff.Width() / scale), MinimumSize().Width(), MaximumSize().Width());
				SetX(__original_position.X() + (__original_size.Width() - new_width) * scale);
			}

		}

		if (new_width != Width() || new_height != Height())
			Resize(std::round(new_width), std::round(new_height));

	}
	bool Window::HasActiveChild() {

		return __panel.ChildManager()->ActiveControl();

	}

}