#include <limits>
#include "hvn3/gui/Control.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/gui/_GuiManager.h"
#include "hvn3/gui/ControlManager.h"
#include "hvn3/gui/StyleManager.h"

// Public members

namespace hvn3 {

	Gui::Control::Control() :
		Control(PointF(0.0f, 0.0f), hvn3::SizeF(0.0f, 0.0f)) {
	}
	Gui::Control::Control(const PointF& location, const hvn3::SizeF& size) :
		Positionable2dBase<float>(location.X(), location.Y()),
		SizeableBase(size.Width(), size.Height()),
		_fixed_pos(location.X(), location.Y()),
		_mouse_last_pos(Mouse::X, Mouse::Y),
		_minimum_size(0.0f, 0.0f),
		_maximum_size(FLT_MAX, FLT_MAX),
		_previous_pos(location.X(), location.Y()),
		_surface(static_cast<int>(Math::Ceiling(size.Width())), static_cast<int>(Math::Ceiling(size.Height()))) {

		_parent = nullptr;
		_manager = nullptr;

		_disposed = false;
		_invalidated = true;
		_visible = true;
		_enabled = true;
		_backcolor = Color::FromArgb(35, 35, 35);
		_forecolor = Color::FromArgb(186, 186, 186);
		_dock = DockStyle::None;
		_anchor = (Gui::ANCHOR)(ANCHOR_LEFT | ANCHOR_TOP);
		_opacity = 1.0f;

		_mouse_is_on = false;
		_mouse_is_down = false;

		Z = 0;

		_prev_focus = false;

	}

	void Gui::Control::OnUpdate(UpdateEventArgs& e) {}
	void Gui::Control::OnDraw(DrawEventArgs& e) {

		if (Invalidated()) {

			// If the Control's size is now invalid, set the bitmap to null.
			if (Width() <= 0.0f || Height() <= 0.0f)
				_surface = Graphics::Bitmap(nullptr, false);
			else if (!_surface || Width() != _surface.Width() || Height() != _surface.Height())
				_surface = Graphics::Bitmap(Width(), Height());

			// Call the Control's OnPaint method to redraw it.
			// Make sure we don't try to draw a Control with dimensions <= 1, because the Graphics object will throw an error when setting the clip.
			if (Width() >= 1.0f && Height() >= 1.0f)
				OnPaint(PaintEventArgs(Graphics::Graphics(_surface)));

		}

		// The Control has now been validated.
		_invalidated = false;

		// Draw the Control's surface bitmap.
		if (_surface)
			e.Graphics().DrawBitmap(X(), Y(), _surface, Color::FromArgbf(Opacity(), Opacity(), Opacity(), Opacity()));

	}
	void Gui::Control::Resize(float width, float height) {

		// Clamp values within the maximum/minimum size.
		width = Math::Clamp(width, _minimum_size.Width(), _maximum_size.Width());
		height = Math::Clamp(height, _minimum_size.Height(), _maximum_size.Height());

		// If the new size is equal to the current size, do not call any events.
		if (width == Width() && height == Height())
			return;

		// Create a resize event args object.
		ResizeEventArgs e(hvn3::SizeF(Width(), Height()), hvn3::SizeF(width, height));

		// Perform the resize.
		SizeableBase::Resize(width, height);

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

		return _forecolor;

	}
	const Color& Gui::Control::BackColor() const {

		return _backcolor;

	}
	void Gui::Control::SetForeColor(const Color& color) {

		_forecolor = color;

	}
	void Gui::Control::SetBackColor(const Color& color) {

		_backcolor = color;

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

		return _opacity;

	}
	void Gui::Control::SetOpacity(float opacity) {

		if (opacity < 0.0f)
			opacity = 0.0f;
		_opacity = opacity;

		Invalidate();

	}

	hvn3::SizeF Gui::Control::MinimumSize() {

		return _minimum_size;

	}
	void Gui::Control::SetMinimumSize(const hvn3::SizeF& size) {

		_minimum_size = size;

		// Resize the control if it is smaller than the minimum size.
		if (Width() < size.Width() || Height() < size.Height())
			Control::Resize(size.Width(), size.Height());

	}
	hvn3::SizeF Gui::Control::MaximumSize() {

		return _maximum_size;

	}
	void Gui::Control::SetMaximumSize(const hvn3::SizeF& size) {

		_maximum_size = size;

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
		_fixed_pos = GetFixedPosition();

	}
	Gui::GuiManager* Gui::Control::Manager() {

		return const_cast<GuiManager*>(static_cast<const Control*>(this)->Manager());

	}
	const Gui::GuiManager* Gui::Control::Manager() const {

		if (_manager == nullptr && Parent() != nullptr)
			return Parent()->Manager();

		return _manager;

	}

	void Gui::Control::BringToFront() {

		// If the Control has a Manager object, allow it to handle the repositioning.
		if (_manager) {
			_manager->ControlManager()->BringToFront(this);
			return;
		}

		// Simply adjust the Z positioning to the minimum possible.
		Z = -INT_MIN;

	}
	void Gui::Control::SendToBack() {

		// If the Control has a Manager object, allow it to handle the repositioning.
		if (_manager) {
			_manager->ControlManager()->SendToBack(this);
			return;
		}

		// Simply adjust the Z positioning to the maximum possible.
		Z = INT_MAX;

	}

	PointF Gui::Control::FixedPosition() const {

		return GetFixedPosition();

		return _fixed_pos;

	}
	RectangleF Gui::Control::Bounds() const {

		return RectangleF(_fixed_pos.X(), _fixed_pos.Y(), Width(), Height());

	}

	float Gui::Control::Scale() const {

		return _manager ? _manager->StyleManager()->DrawingScale() : 1.0f;

	}

	bool Gui::Control::IsActiveControl() {

		if (_manager == nullptr)
			return false;

		return _manager->ControlManager()->ActiveControl() == this;

	}

	void Gui::Control::SetX(float x) {

		PointF old_position(X(), Y());

		Positionable2dBase::SetX(x);

		OnMove(MoveEventArgs(old_position));

	}
	void Gui::Control::SetY(float y) {

		PointF old_position(X(), Y());

		Positionable2dBase::SetY(y);

		OnMove(MoveEventArgs(old_position));

	}
	void Gui::Control::SetPosition(float x, float y) {

		PointF old_position(X(), Y());

		Positionable2dBase::SetPosition(x, y);

		OnMove(MoveEventArgs(old_position));

	}

	void Gui::Control::OnMouseLeave() {}
	void Gui::Control::OnMouseEnter() {}
	void Gui::Control::OnMouseHover() {}
	void Gui::Control::OnMouseDown(MouseEventArgs& e) {}
	void Gui::Control::OnMouseUp(MouseEventArgs& e) {}
	void Gui::Control::OnMouseMove(MouseMoveEventArgs& e) {}
	void Gui::Control::OnClick() {}
	void Gui::Control::OnDoubleClick() {}
	void Gui::Control::OnPaint(PaintEventArgs& e) {}
	void Gui::Control::OnResize(ResizeEventArgs& e) {}
	void Gui::Control::OnMove(MoveEventArgs& e) {

		// Update fixed coordinates (relative to view origin).
		_fixed_pos = GetFixedPosition();

	}
	void Gui::Control::OnGotFocus() {}
	void Gui::Control::OnLostFocus() {}
	void Gui::Control::OnKeyDown() {}
	void Gui::Control::OnKeyPressed() {}
	void Gui::Control::OnKeyReleased() {}
	void Gui::Control::OnManagerChanged(ManagerChangedEventArgs& e) {

		// Re-apply dockstyle for the new manager's dockable region.
		//if (Manager() != nullptr)
		//	ApplyDockStyle();

	}
	void Gui::Control::OnEnabledChanged(EnabledChangedEventArgs& e) {}

	// Private members

	bool Gui::Control::HasActiveChild() {

		return false;

	}
	PointF Gui::Control::GetFixedPosition() const {

		PointF fp(X(), Y());

		if (Parent())
			fp.Offset(Parent()->FixedPosition().X(), Parent()->FixedPosition().Y());

		// Apply the control manager's control offset.
		if (Manager() && Manager()->ControlManager())
			fp.Offset(Manager()->ControlManager()->ControlOffset().X(), Manager()->ControlManager()->ControlOffset().Y());

		return fp;

	}
	void Gui::Control::ApplyDockStyle() {



	}

}