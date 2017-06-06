#include <limits>
#include "Control.h"
#include "io/Mouse.h"
#include "gui/GuiManager.h"
#include "gui/ControlManager.h"
#include "gui/StyleManager.h"

// Public members

namespace hvn3 {

	Gui::Control::Control() :
		Control(Point(0.0f, 0.0f), Size(0.0f, 0.0f)) {
	}
	Gui::Control::Control(const Point& location, const Size& size) :
		IPositionable(location.X(), location.Y()),
		ISizeable(size.Width(), size.Height()),
		__fixed_pos(location.X(), location.Y()),
		__mouse_last_pos(Mouse::X, Mouse::Y),
		__minimum_size(0.0f, 0.0f),
		__maximum_size(FLT_MAX, FLT_MAX),
		__previous_pos(location.X(), location.Y()),
		_surface(size.Width(), size.Height()) {

		_parent = nullptr;
		__manager = nullptr;

		_disposed = false;
		_invalidated = true;
		_visible = true;
		_enabled = true;
		__backcolor = Color::FromArgb(35, 35, 35);
		__forecolor = Color::FromArgb(186, 186, 186);
		_dock = DockStyle::None;
		_anchor = (Gui::ANCHOR)(ANCHOR_LEFT | ANCHOR_TOP);
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
			e.Graphics().DrawBitmap(X(), Y(), &_surface, Color::FromArgbf(Opacity(), Opacity(), Opacity(), Opacity()));

	}
	void Gui::Control::Resize(float width, float height) {

		// Clamp values within the maximum/minimum size.
		width = Clamp(width, __minimum_size.Width(), __maximum_size.Width());
		height = Clamp(height, __minimum_size.Height(), __maximum_size.Height());

		// If the new size is equal to the current size, do not call any events.
		if (width == Width() && height == Height())
			return;

		// Create a resize event args object.
		ResizeEventArgs e(Size(Width(), Height()), Size(width, height));

		// Perform the resize.
		ISizeable::Resize(width, height);

		// Trigger the resize event for the control.
		OnResize(e);

		// Invalidate the control so that it can be redrawn.
		Invalidate();

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

		_disposed = true;

	}
	bool Gui::Control::IsDisposed() {

		return _disposed;

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

	int Gui::Control::Anchors() {

		return _anchor;

	}
	void Gui::Control::SetAnchors(int anchors) {

		_anchor = anchors;

	}
	Gui::DockStyle Gui::Control::Dock() const {

		return _dock;

	}
	void Gui::Control::SetDock(DockStyle dock) {

		// Update the control's dock style.
		_dock = dock;

		// If the control is already tied to a manager, apply the dock style.
		if (Manager() != nullptr)
			ApplyDockStyle();

	}

	float Gui::Control::Opacity() {

		return __opacity;

	}
	void Gui::Control::SetOpacity(float opacity) {

		if (opacity < 0.0f)
			opacity = 0.0f;
		__opacity = opacity;

		Invalidate();

	}

	Size Gui::Control::MinimumSize() {

		return __minimum_size;

	}
	void Gui::Control::SetMinimumSize(const Size& size) {

		__minimum_size = size;

		// Resize the control if it is smaller than the minimum size.
		if (Width() < size.Width() || Height() < size.Height())
			Control::Resize(size.Width(), size.Height());

	}
	Size Gui::Control::MaximumSize() {

		return __maximum_size;

	}
	void Gui::Control::SetMaximumSize(const Size& size) {

		__maximum_size = size;

		// Resize the control if it is larger than the maximum size.
		if (Width() > size.Width() || Height() > size.Height())
			Control::Resize(size.Width(), size.Height());

	}

	bool Gui::Control::Visible() {

		return _visible;

	}
	void Gui::Control::SetVisible(bool is_visible) {

		_visible = is_visible;

	}

	bool Gui::Control::Enabled() {

		return _enabled;

	}
	void Gui::Control::SetEnabled(bool is_enabled) {

		_enabled = is_enabled;

		OnEnabledChanged(EnabledChangedEventArgs(is_enabled));

		// Invalidate Control in case it has a different appearance when enabled/disabled.
		Invalidate();

	}

	Gui::Control* Gui::Control::Parent() {

		return const_cast<Control*>(static_cast<const Control*>(this)->Parent());

	}
	const Gui::Control* Gui::Control::Parent() const {

		return _parent;

	}
	void Gui::Control::SetParent(Control* parent) {

		_parent = parent;

		// Update the control's position relative to its parent.
		__fixed_pos = GetFixedPosition();

	}
	Gui::GuiManager* Gui::Control::Manager() {

		return const_cast<GuiManager*>(static_cast<const Control*>(this)->Manager());

	}
	const Gui::GuiManager* Gui::Control::Manager() const {

		if (__manager == nullptr && Parent() != nullptr)
			return Parent()->Manager();

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

	Point Gui::Control::FixedPosition() const {

		return GetFixedPosition();

		return __fixed_pos;

	}
	Rectangle Gui::Control::Bounds() const {

		return Rectangle(__fixed_pos.X(), __fixed_pos.Y(), Width(), Height());

	}

	float Gui::Control::Scale() const {

		return __manager ? __manager->StyleManager()->DrawingScale() : 1.0f;

	}

	bool Gui::Control::IsActiveControl() {

		if (__manager == nullptr)
			return false;

		return __manager->ControlManager()->ActiveControl() == this;

	}

	void Gui::Control::SetX(float x) {

		Point old_position(X(), Y());

		IPositionable::SetX(x);

		OnMove(MoveEventArgs(old_position));

	}
	void Gui::Control::SetY(float y) {

		Point old_position(X(), Y());

		IPositionable::SetY(y);

		OnMove(MoveEventArgs(old_position));

	}
	void Gui::Control::SetXY(float x, float y) {

		Point old_position(X(), Y());

		IPositionable::SetXY(x, y);

		OnMove(MoveEventArgs(old_position));

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
	void Gui::Control::OnResize(ResizeEventArgs& e) {}
	void Gui::Control::OnMove(MoveEventArgs& e) {

		// Update fixed coordinates (relative to view origin).
		__fixed_pos = GetFixedPosition();

	}
	void Gui::Control::OnGotFocus() {}
	void Gui::Control::OnLostFocus() {}
	void Gui::Control::OnKeyDown() {}
	void Gui::Control::OnKeyPressed() {}
	void Gui::Control::OnKeyReleased() {}
	void Gui::Control::OnManagerChanged(ManagerChangedEventArgs& e) {

		// Re-apply dockstyle for the new manager's dockable region.
		if (Manager() != nullptr)
			ApplyDockStyle();

	}
	void Gui::Control::OnEnabledChanged(EnabledChangedEventArgs& e) {}

	// Private members

	bool Gui::Control::HasActiveChild() {

		return false;

	}
	Point Gui::Control::GetFixedPosition() const {

		Point fp(X(), Y());

		if (Parent())
			fp.Translate(Parent()->FixedPosition().X(), Parent()->FixedPosition().Y());

		// Apply the control manager's control offset.
		if (Manager() && Manager()->ControlManager())
			fp.Translate(Manager()->ControlManager()->ControlOffset().X(), Manager()->ControlManager()->ControlOffset().Y());

		return fp;

	}
	void Gui::Control::ApplyDockStyle() {

		switch (_dock) {

		case DockStyle::Top:
			SetXY(0, 0);
			SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP);
			SetWidth(Manager()->DockableRegion().Width());
			break;

		}

	}

}