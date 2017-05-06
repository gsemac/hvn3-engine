#include <limits>
#include "Control.h"
#include "io/Mouse.h"
#include "GUIManager.h"

// Private members

bool Gui::Control::HasActiveChild() {

	return false;

}
Point Gui::Control::GetFixedPosition() {

	Point p(X(), Y());
	Control* control = this;

	while (control->Parent()) {
		control = control->Parent();
		p.Translate(control->X(), control->Y());
	}

	return p;

}

// Public members

Gui::Control::Control() :
	Control(Point(0.0f, 0.0f), Size(0.0f, 0.0f)) {}
Gui::Control::Control(const Point& location, const Size& size) :
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
	__backcolor = Color::FromArgb(35, 35, 35);
	__forecolor = Color::FromArgb(186, 186, 186);
	__anchor = (Gui::ANCHOR)(ANCHOR_LEFT | ANCHOR_TOP);
	__opacity = 1.0f;

	__mouse_is_on = false;
	__mouse_is_down = false;

	Z = 0;

	__prev_focus = false;

}

void Gui::Control::Update(UpdateEventArgs& e) {}
void Gui::Control::Draw(DrawEventArgs& e) {

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
void Gui::Control::Resize(float width, float height) {

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

void Gui::Control::Invalidate() {

	__invalidated = true;

	if (__parent) __parent->Invalidate();

}
bool Gui::Control::Invalidated() {

	return __invalidated;

}
void Gui::Control::Dispose() {

	__disposed = true;

}
bool Gui::Control::IsDisposed() {

	return __disposed;

}

const Color& Gui::Control::ForeColor() const {

	return __forecolor;

}
const Color& Gui::Control::BackColor() const {

	return __backcolor;

}
void Gui::Control::SetForeColor(const Color& color) {

	__forecolor = color;

}
void Gui::Control::SetBackColor(const Color& color) {

	__backcolor = color;

}

Gui::ANCHOR Gui::Control::Anchors() {

	return __anchor;

}
void Gui::Control::SetAnchors(Gui::ANCHOR anchors) {

	__anchor = anchors;

}

float Gui::Control::Opacity() {

	return __opacity;

}
void Gui::Control::SetOpacity(float opacity) {

	__opacity = opacity;

}

Size Gui::Control::MinimumSize() {

	return __minimum_size;

}
void Gui::Control::SetMinimumSize(const Size& size) {

	__minimum_size = size;

}
Size Gui::Control::MaximumSize() {

	return __maximum_size;

}
void Gui::Control::SetMaximumSize(const Size& size) {

	__maximum_size = size;

}

bool Gui::Control::Visible() {

	return __visible;

}
void Gui::Control::SetVisible(bool is_visible) {

	__visible = is_visible;

}

bool Gui::Control::Enabled() {

	return __enabled;

}
void Gui::Control::SetEnabled(bool is_enabled) {

	__enabled = is_enabled;

	// Invalidate Control in case it has a different appearance when enabled/disabled.
	Invalidate();

}

Gui::Control* Gui::Control::Parent() {

	return __parent;

}
void Gui::Control::SetParent(Control* parent) {

	__parent = parent;

}
Gui::GuiManager* Gui::Control::Manager() {

	return __manager;

}

void Gui::Control::BringToFront() {

	// If the Control has a Manager object, allow it to handle the repositioning.
	if (__manager) {
		__manager->BringToFront(this);
		return;
	}

	// Simply adjust the Z positioning to the minimum possible.
	Z = -INT_MAX;

}
void Gui::Control::SendToBack() {

	// If the Control has a Manager object, allow it to handle the repositioning.
	if (__manager) {
		__manager->SendToBack(this);
		return;
	}

	// Simply adjust the Z positioning to the maximum possible.
	Z = INT_MAX;

}

Point Gui::Control::FixedPosition() {

	return __fixed_pos;

}
Rectangle Gui::Control::Bounds() {

	return Rectangle(__fixed_pos.X(), __fixed_pos.Y(), Width(), Height());

}

float Gui::Control::Scale() const {

	return __manager ? __manager->Scale() : 1.0f;

}

void Gui::Control::OnMouseLeave() {}
void Gui::Control::OnMouseEnter() {}
void Gui::Control::OnMouseHover() {}
void Gui::Control::OnMouseDown() {}
void Gui::Control::OnMouseUp() {}
void Gui::Control::OnMouseMove() {}
void Gui::Control::OnClick() {}
void Gui::Control::OnDoubleClick() {}
void Gui::Control::OnPaint(PaintEventArgs& e) {}
void Gui::Control::OnResize() {}
void Gui::Control::OnMove() {

	// Update fixed coordinates (relative to view origin).
	__fixed_pos = GetFixedPosition();

}
void Gui::Control::OnGotFocus() {}
void Gui::Control::OnLostFocus() {}
void Gui::Control::OnKeyDown() {}
void Gui::Control::OnKeyPressed() {}
void Gui::Control::OnKeyReleased() {}