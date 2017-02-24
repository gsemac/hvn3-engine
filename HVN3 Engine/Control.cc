#include <limits>
#include "Control.h"
#include "Mouse.h"
#include "GUITheme.h"
#include "GUIManager.h"

// Private members

bool GUI::Control::HasActiveChild() {

	return false;

}
Point GUI::Control::GetFixedPosition() {

	Point p(X(), Y());
	Control* control = this;

	while (control->Parent()) {
		control = control->Parent();
		p.Translate(control->X(), control->Y());
	}

	return p;

}

// Public members

GUI::Control::Control() :
	Control(Point(0.0f, 0.0f), Size(0.0f, 0.0f)) {}
GUI::Control::Control(const Point& location, const Size& size) :
	IPositionable(location.X(), location.Y()),
	ISizeable(size.Width(), size.Height()),
	__fixed_pos(location.X(), location.Y()),
	__mouse_last_pos(Mouse::X, Mouse::Y),
	__minimum_size(0.0f, 0.0f),
	__maximum_size(FLT_MAX, FLT_MAX),
	__previous_pos(location.X(), location.Y()),
	__surface(size.Width(), size.Height())
{

	__parent = nullptr;
	__manager = nullptr;

	__disposed = false;
	__invalidated = true;
	__visible = true;
	__enabled = true;
	__backcolor = GUI::ActiveTheme()->PrimaryColor();
	__forecolor = GUI::ActiveTheme()->SecondaryColor();
	__anchor = (GUI::ANCHOR)(ANCHOR_LEFT | ANCHOR_TOP);
	__opacity = 1.0f;

	__mouse_is_on = false;
	__mouse_is_down = false;

	Z = 0;

	__prev_focus = false;

}

void GUI::Control::Update(UpdateEventArgs& e) {}
void GUI::Control::Draw(DrawEventArgs& e) {

	if (__invalidated) {

		// If the Control's size is now invalid, set the bitmap to null.
		if (Width() <= 0.0f || Height() <= 0.0f)
			__surface = Drawing::Bitmap(nullptr, false);
		else if (!__surface || Width() != __surface.Width() || Height() != __surface.Height())
			__surface = Drawing::Bitmap(Width(), Height());

		// Call the Control's OnPaint method to redraw it.
		// Make sure we don't try to draw a Control with dimensions <= 1, because the Graphics object will throw an error when setting the clip.
		if (Width() >= 1.0f && Height() >= 1.0f)
			OnPaint(PaintEventArgs(Drawing::Graphics(__surface)));

	}

	// The Control has now been validated. This isn't important right now, but will be for future drawing optimizations.
	__invalidated = false;

	// Draw the Control's surface bitmap.
	if (__surface)
		e.Graphics().DrawBitmap(X(), Y(), __surface);

	/*
	if (Invalidated()) {

		// When a Control is "Invalidated", that means it needs to be redrawn.

		// If the size of the Control has changed, or its size has become invalid, create a new Bitmap.
		if (Width() <= 0.0f || Height() <= 0.0f)
			__bmp = Drawing::Bitmap(0, 0);
		else if (!__bmp || (Width() != __bmp.Width() || Height() != __bmp.Height()))
			__bmp = Drawing::Bitmap(Width(), Height());

		// If the Control's Bitmap is valid, redraw by calling OnPaint event.
		if (__bmp) {

			Point pos(X(), Y());
			__graphics.Clear(Color::Transparent);
			SetXY(0.0f, 0.0f);
			OnPaint();
			SetXY(pos.X(), pos.Y());

		}

		// The Control is now validated.
		__invalidated = false;

	}

	// Draw the Control's bitmap image (if it exists).
	if (__bmp) {
		float scale = __manager ? __manager->Scale() : 1.0f;
		al_draw_tinted_scaled_bitmap(
			__bmp,
			al_map_rgba_f(1.0f * __opacity, 1.0f * __opacity, 1.0f * __opacity, __opacity),
			0.0f,
			0.0f,
			Width(),
			Height(),
			X(),
			Y(),
			Width() * scale,
			Height() * scale,
			NULL
		);

	}

	*/

}
void GUI::Control::Resize(float width, float height) {

	// Clamp values within the maximum/minimum size.
	width = Clamp(width, __minimum_size.Width(), __maximum_size.Width());
	height = Clamp(height, __minimum_size.Height(), __maximum_size.Height());

	// If the new size is equal to the current size, do not call any events.
	if (width == Width() && height == Height())
		return;

	// Perform the resize.
	ISizeable::Resize(width, height);

	// Call related events.
	Invalidate();
	OnResize();

}

void GUI::Control::Invalidate() {

	__invalidated = true;

	if (__parent) __parent->Invalidate();

}
bool GUI::Control::Invalidated() {

	return __invalidated;

}
void GUI::Control::Dispose() {

	__disposed = true;

}
bool GUI::Control::IsDisposed() {

	return __disposed;

}

const Color& GUI::Control::ForeColor() const {

	return __forecolor;

}
const Color& GUI::Control::BackColor() const {

	return __backcolor;

}
void GUI::Control::SetForeColor(const Color& color) {

	__forecolor = color;

}
void GUI::Control::SetBackColor(const Color& color) {

	__backcolor = color;

}

GUI::ANCHOR GUI::Control::Anchors() {

	return __anchor;

}
void GUI::Control::SetAnchors(GUI::ANCHOR anchors) {

	__anchor = anchors;

}

float GUI::Control::Opacity() {

	return __opacity;

}
void GUI::Control::SetOpacity(float opacity) {

	__opacity = opacity;

}

Size GUI::Control::MinimumSize() {

	return __minimum_size;

}
void GUI::Control::SetMinimumSize(const Size& size) {

	__minimum_size = size;

}
Size GUI::Control::MaximumSize() {

	return __maximum_size;

}
void GUI::Control::SetMaximumSize(const Size& size) {

	__maximum_size = size;

}

bool GUI::Control::Visible() {

	return __visible;

}
void GUI::Control::SetVisible(bool is_visible) {

	__visible = is_visible;

}

bool GUI::Control::Enabled() {

	return __enabled;

}
void GUI::Control::SetEnabled(bool is_enabled) {

	__enabled = is_enabled;

	// Invalidate Control in case it has a different appearance when enabled/disabled.
	Invalidate();

}

GUI::Control* GUI::Control::Parent() {

	return __parent;

}
void GUI::Control::SetParent(Control* parent) {

	__parent = parent;

}
GUI::GuiManager* GUI::Control::Manager() {

	return __manager;

}

void GUI::Control::BringToFront() {

	// If the Control has a Manager object, allow it to handle the repositioning.
	if (__manager) {
		__manager->BringToFront(this);
		return;
	}

	// Simply adjust the Z positioning to the minimum possible.
	Z = -INT_MAX;

}
void GUI::Control::SendToBack() {

	// If the Control has a Manager object, allow it to handle the repositioning.
	if (__manager) {
		__manager->SendToBack(this);
		return;
	}

	// Simply adjust the Z positioning to the maximum possible.
	Z = INT_MAX;

}

Point GUI::Control::FixedPosition() {

	return __fixed_pos;

}
Rectangle GUI::Control::Bounds() {

	return Rectangle(__fixed_pos.X(), __fixed_pos.Y(), Width(), Height());

}

float GUI::Control::Scale() const {

	return __manager ? __manager->Scale() : 1.0f;

}

void GUI::Control::OnMouseLeave() {}
void GUI::Control::OnMouseEnter() {}
void GUI::Control::OnMouseHover() {}
void GUI::Control::OnMouseDown() {}
void GUI::Control::OnMouseUp() {}
void GUI::Control::OnMouseMove() {}
void GUI::Control::OnClick() {}
void GUI::Control::OnDoubleClick() {}
void GUI::Control::OnPaint(PaintEventArgs& e) {}
void GUI::Control::OnResize() {}
void GUI::Control::OnMove() {

	// Update fixed coordinates (relative to view origin).
	__fixed_pos = GetFixedPosition();

}
void GUI::Control::OnGotFocus() {}
void GUI::Control::OnLostFocus() {}
void GUI::Control::OnKeyDown() {}
void GUI::Control::OnKeyPressed() {}
void GUI::Control::OnKeyReleased() {}