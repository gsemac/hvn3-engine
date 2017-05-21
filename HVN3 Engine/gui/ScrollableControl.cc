#include "gui/ScrollableControl.h"
#include "gui/Control.h"
#include "gui/GuiManager.h"
#include "gui/Scrollbar.h"
#define SCROLLBAR_DEFAULT_WIDTH 12

namespace Gui {

	ScrollableControl::ScrollableControl(float scroll_height) :
		ScrollableControl(Size(Width(), scroll_height)) {
	}
	ScrollableControl::ScrollableControl(const Size& scrollable_region) :
		IScrollable(this, scrollable_region) {

		_scrollbars[VERTICAL] = nullptr;
		_scrollbars[HORIZONTAL] = nullptr;

	}
	ScrollableControl::~ScrollableControl() {

		// If the control was never added to a control manager, scrollbars would've never been created.
		if (!Manager() || !Manager()->ControlManager())
			return;

		// Remove scrollbars from the manager, otherwise they'll persist after the panel is removed.
		if (_scrollbars[VERTICAL])
			Manager()->ControlManager()->RemoveControl(_scrollbars[VERTICAL]);
		if (_scrollbars[HORIZONTAL])
			Manager()->ControlManager()->RemoveControl(_scrollbars[HORIZONTAL]);

		_scrollbars[VERTICAL] = nullptr;
		_scrollbars[HORIZONTAL] = nullptr;

	}

	Handle<Scrollbar> ScrollableControl::VerticalScrollbar() {

		return _scrollbars[VERTICAL];

	}
	Handle<Scrollbar> ScrollableControl::HorizontalScrollbar() {

		return _scrollbars[HORIZONTAL];

	}

	bool ScrollableControl::HasVerticalScrollbar() const {

		return _scrollbars[VERTICAL] != nullptr;

	}
	bool ScrollableControl::HasHorizontalScrollbar() const {

		return _scrollbars[HORIZONTAL] != nullptr;

	}

	void ScrollableControl::OnResize(ResizeEventArgs& e) {

		// Store the new width and height of the control.
		float vwidth = Width();
		float vheight = Height();

		// If either scrollbar is visible (or going to be), adjust the visible region accordingly.
		bool vscroll_accounted_for = false;
		if (ScrollableRegion().Height() > vheight && _scrollbars[VERTICAL]) {
			vwidth -= VerticalScrollbar()->Width();
			vscroll_accounted_for = true;
		}
		if (ScrollableRegion().Width() > vwidth && _scrollbars[HORIZONTAL]) {
			vheight -= HorizontalScrollbar()->Height();
			// What if by decreasing the height, we now need to show the vertical scrollbar?
			// If the vertical scrollbar is going to be visible and we haven't already assumed it is, decrease width of visible region.
			if (!vscroll_accounted_for && ScrollableRegion().Height() > vheight && _scrollbars[VERTICAL])
				vwidth -= VerticalScrollbar()->Width();
		}

		// Update the visible region of the control.
		SetVisibleRegion(Size(vwidth, vheight));

		// Update the state of the scrollbars themselves.
		UpdateScrollbars();

		Control::OnResize(e);

	}
	void ScrollableControl::OnPaint(PaintEventArgs& e) {

		// If both scrollbars are visible, fill in the space between the two scrollbars.
		if (VerticalScrollbar() && VerticalScrollbar()->Visible() && HorizontalScrollbar() && HorizontalScrollbar()->Visible())
			e.Graphics().DrawFilledRectangle(Width() - VerticalScrollbar()->Width(), Height() - HorizontalScrollbar()->Height(), VerticalScrollbar()->Width(), HorizontalScrollbar()->Height(), VerticalScrollbar()->BackColor());

		Control::OnPaint(e);

	}
	void ScrollableControl::OnMove(MoveEventArgs& e) {

		// Update the position of the scrollbars to match the control's new position.
		UpdateScrollbars();

		Control::OnMove(e);

	}
	void ScrollableControl::OnManagerChanged(ManagerChangedEventArgs& e) {

		// If the new manager is null, do nothing but set scrollbars to null. They would've been deleted by the previous manager's control manager.
		if (Manager() == nullptr) {
			_scrollbars[VERTICAL] = nullptr;
			_scrollbars[HORIZONTAL] = nullptr;
			return;
		}

		// If scrollbars haven't been created yet, create them.
		if (_scrollbars[VERTICAL] == nullptr)
			_scrollbars[VERTICAL] = new Scrollbar(this, Point(X() + Width() - SCROLLBAR_DEFAULT_WIDTH, Y() + SCROLLBAR_DEFAULT_WIDTH), Size(SCROLLBAR_DEFAULT_WIDTH, Height() - SCROLLBAR_DEFAULT_WIDTH), Orientation::Vertical);
		if (_scrollbars[HORIZONTAL] == nullptr)
			_scrollbars[HORIZONTAL] = new Scrollbar(this, Point(X(), Y() + Height() - SCROLLBAR_DEFAULT_WIDTH), Size(Width() - SCROLLBAR_DEFAULT_WIDTH, SCROLLBAR_DEFAULT_WIDTH), Orientation::Horizontal);

		// Move scrollbars from the previous manager to the new manager, if the previous manager was non-null.
		if (e.PreviousManager() != nullptr) {
			e.PreviousManager()->ControlManager()->MoveControl(_scrollbars[VERTICAL], Manager()->ControlManager());
			e.PreviousManager()->ControlManager()->MoveControl(_scrollbars[HORIZONTAL], Manager()->ControlManager());
		}

		// Otherwise, just add them to the new manager.
		else {
			Manager()->ControlManager()->AddControl(Control::Create(_scrollbars[VERTICAL]));
			Manager()->ControlManager()->AddControl(Control::Create(_scrollbars[HORIZONTAL]));
		}

		// Initialize the state of the scrollbars.
		UpdateScrollbars();

		Control::OnManagerChanged(e);

	}

	// Private methods

	void ScrollableControl::UpdateScrollbars() {

		Point fp = Point(X(), Y()); //FixedPosition(); ? 

		bool vscroll_visible = _scrollbars[VERTICAL] != nullptr && VisibleRegion().Height() < ScrollableRegion().Height();
		bool hscroll_visible = _scrollbars[HORIZONTAL] != nullptr && VisibleRegion().Width() < ScrollableRegion().Width();

		if (_scrollbars[VERTICAL] != nullptr) {
			_scrollbars[VERTICAL]->Resize(
				_scrollbars[VERTICAL]->Width(),
				Height() - (hscroll_visible ? _scrollbars[HORIZONTAL]->Height() : 0.0f)
				);
			_scrollbars[VERTICAL]->SetXY(fp.X() + Width() - _scrollbars[VERTICAL]->Width(), fp.Y());
			_scrollbars[VERTICAL]->SetVisible(vscroll_visible);
		}

		if (_scrollbars[HORIZONTAL] != nullptr) {
			_scrollbars[HORIZONTAL]->Resize(
				Width() - (vscroll_visible ? _scrollbars[VERTICAL]->Width() : 0.0f),
				_scrollbars[HORIZONTAL]->Height()
				);
			_scrollbars[HORIZONTAL]->SetXY(fp.X(), fp.Y() + Height() - _scrollbars[HORIZONTAL]->Height());
			_scrollbars[HORIZONTAL]->SetVisible(hscroll_visible);
		}

	}

}