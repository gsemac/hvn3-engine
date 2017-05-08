#include <limits>
#include "Control.h"
#include "io/Mouse.h"
#include "gui/GuiManager.h"
#include "gui/ControlManager.h"
#include "gui/StyleManager.h"

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
	_surface(size.Width(), size.Height())
{

	_parent = nullptr;
	__manager = nullptr;

	__disposed = false;
	_invalidated = true;
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

	if (Invalidated()) {

		// If the Control's size is now invalid, set the bitmap to null.
		if (Width() <= 0.0f || Height() <= 0.0f)
			_surface = Drawing::Bitmap(nullptr, false);
		else if (!_surface || Width() != _surface.Width() || Height() != _surface.Height())
			_surface = Drawing::Bitmap(Width(), Height());

		// Call the Control's OnPaint method to redraw it.
		// Make sure we don't try to draw a Control with dimensions <= 1, because the Graphics object will throw an error when setting the clip.
		if (Width() >= 1.0f && Height() >= 1.0f)
			OnPaint(PaintEventArgs(Drawing::Graphics(_surface)));

	}

	// The Control has now been validated.
	_invalidated = false;

	// Draw the Control's surface bitmap.
	if (_surface)
		e.Graphics().DrawBitmap(X(), Y(), _surface, Color::FromArgbf(Opacity(), Opacity(), Opacity(), Opacity()));

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

	std::cout << Invalidated();

}

void Gui::Control::Invalidate() {

	_invalidated = true;

	if (_parent) 
		_parent->Invalidate();

}
bool Gui::Control::Invalidated() {

	return _invalidated;

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

	if (opacity < 0.0f)
		opacity = 0.0f;
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

	return _parent;

}
void Gui::Control::SetParent(Control* parent) {

	_parent = parent;

}
Gui::GuiManager* Gui::Control::Manager() {

	return __manager;

}

void Gui::Control::BringToFront() {

	// If the Control has a Manager object, allow it to handle the repositioning.
	if (__manager) {
		__manager->ControlManager()->BringToFront(this);
		return;
	}

	// Simply adjust the Z positioning to the minimum possible.
	Z = -INT_MAX;

}
void Gui::Control::SendToBack() {

	// If the Control has a Manager object, allow it to handle the repositioning.
	if (__manager) {
		__manager->ControlManager()->SendToBack(this);
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

	return __manager ? __manager->StyleManager()->DrawingScale() : 1.0f;

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