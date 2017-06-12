#pragma once
#include "Panel.h"
#include "Scrollbar.h"
#define SCROLLBAR_DEFAULT_WIDTH 12

namespace hvn3 {

	namespace Gui {

		Panel::Panel(const Point2F& position, const SizeF& dimensions) :
			Panel(position, dimensions, dimensions) {
		}
		Panel::Panel(const Point2F& position, const SizeF& dimensions, const SizeF& scrollable_region) :
			Control(position, dimensions),
			IContainer(this),
			IScrollable(this, scrollable_region) {

			_scrollbars[0] = nullptr;
			_scrollbars[1] = nullptr;

		}
		Panel::~Panel() {

			// Remove scrollbars from the manager.
			// This is important, because a panel could be removed from a control manager without destroying the control manager.

			if (!Manager() || !Manager()->ControlManager())
				return;

			if (_scrollbars[VERTICAL])
				Manager()->ControlManager()->RemoveControl(_scrollbars[VERTICAL]);

			if (_scrollbars[HORIZONTAL])
				Manager()->ControlManager()->RemoveControl(_scrollbars[HORIZONTAL]);

		}

		void Panel::OnScroll(ScrollEventArgs& e) {

			// For each child control, we want it to be shifted by the scroll offset.
			if (e.Orientation() == Orientation::Vertical)
				Controls()->SetControlOffset(Controls()->ControlOffset().X(), Round(-e.Position()));
			else
				Controls()->SetControlOffset(Round(-e.Position()), Controls()->ControlOffset().Y());

			IScrollable::OnScroll(e);

			Invalidate();

		}
		void Panel::OnResize(ResizeEventArgs& e) {

			// Calculate the region in which all controls exist.
			RectangleF control_region = GetControlBounds();
			SizeF old_scrollable_region = ScrollableRegion();

			// Normally, the visible region would be the width/height of the panel.
			float vwidth = Width();
			float vheight = Height();

			// However, we need to take into account whether or not the scrollbars will be visible to get the final visible region.
			if (control_region.Height() > vheight && VerticalScrollbar())
				vwidth -= VerticalScrollbar()->Width();
			if (control_region.Width() > vwidth && HorizontalScrollbar())
				vheight -= HorizontalScrollbar()->Height();

			// Now we can set the visible region.
			SetVisibleRegion(SizeF(vwidth, vheight));

			// We can also set the scrollable region, which will be the control region.
			SetScrollableRegion(SizeF(Max(VisibleRegion().Width(), control_region.Width()), Max(VisibleRegion().Height(), control_region.Height())));

			// Update the positions/sizes of the scrollbars.
			UpdateScrollbarPositionsAndSizes();

			UpdateAnchors(ResizeEventArgs(old_scrollable_region, ScrollableRegion()));

		}
		void Panel::OnPaint(PaintEventArgs& e) {

			// Draw the background color of the control.
			e.Graphics().Clear(BackColor());

			// Draw child controls.
			Controls()->OnDraw(e);

			// If both scrollbars are visible, fill in the space between the two scrollbars.
			if (VerticalScrollbar() && VerticalScrollbar()->Visible() && HorizontalScrollbar() && HorizontalScrollbar()->Visible())
				e.Graphics().DrawFilledRectangle(Width() - VerticalScrollbar()->Width(), Height() - HorizontalScrollbar()->Height(), VerticalScrollbar()->Width(), HorizontalScrollbar()->Height(), VerticalScrollbar()->BackColor());

		}
		void Panel::OnMove(MoveEventArgs& e) {

			// Update the position of the scrollbars to match the control's new position.
			UpdateScrollbarPositionsAndSizes();

			Control::OnMove(e);

		}
		void Panel::OnUpdate(UpdateEventArgs& e) {

			// Update child Controls.
			if (IsActiveControl())
				Controls()->OnUpdate(e);
			else
				Controls()->ClearActiveControl();

		}
		void Panel::OnManagerChanged(ManagerChangedEventArgs& e) {

			// If the new manager is null, do nothing but set scrollbars to null. 
			// They would've been deleted by the previous manager's control manager.
			if (Manager() == nullptr) {
				_scrollbars[VERTICAL] = nullptr;
				_scrollbars[HORIZONTAL] = nullptr;
				return;
			}

			// If scrollbars haven't been created yet, create them.
			if (_scrollbars[VERTICAL] == nullptr)
				_scrollbars[VERTICAL] = new Scrollbar(this, Point2F(X() + Width() - SCROLLBAR_DEFAULT_WIDTH, Y() + SCROLLBAR_DEFAULT_WIDTH), SizeF(SCROLLBAR_DEFAULT_WIDTH, Height() - SCROLLBAR_DEFAULT_WIDTH), Orientation::Vertical);
			if (_scrollbars[HORIZONTAL] == nullptr)
				_scrollbars[HORIZONTAL] = new Scrollbar(this, Point2F(X(), Y() + Height() - SCROLLBAR_DEFAULT_WIDTH), SizeF(Width() - SCROLLBAR_DEFAULT_WIDTH, SCROLLBAR_DEFAULT_WIDTH), Orientation::Horizontal);

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

			UpdateScrollbarPositionsAndSizes();

		}
		void Panel::OnClick() {

			std::cout << "Panel clicked!\n";

		}

		// Private methods

		bool Panel::HasActiveChild() {

			return Controls()->ActiveControl();

		}

		void Panel::UpdateScrollbarPositionsAndSizes() {

			Point2F fp = Point2F(X(), Y());//FixedPosition(); ? 

			bool vscroll_visible = _scrollbars[VERTICAL] != nullptr && VisibleRegion().Height() < ScrollableRegion().Height();
			bool hscroll_visible = _scrollbars[HORIZONTAL] != nullptr && VisibleRegion().Width() < ScrollableRegion().Width();

			if (_scrollbars[VERTICAL] != nullptr) {
				_scrollbars[VERTICAL]->Resize(
					_scrollbars[VERTICAL]->Width(),
					Height() - (hscroll_visible ? _scrollbars[HORIZONTAL]->Height() : 0.0f)
					);
				_scrollbars[VERTICAL]->SetPosition(fp.X() + Width() - _scrollbars[VERTICAL]->Width(), fp.Y());
				_scrollbars[VERTICAL]->SetVisible(vscroll_visible);
			}

			if (_scrollbars[HORIZONTAL] != nullptr) {
				_scrollbars[HORIZONTAL]->Resize(
					Width() - (vscroll_visible ? _scrollbars[VERTICAL]->Width() : 0.0f),
					_scrollbars[HORIZONTAL]->Height()
					);
				_scrollbars[HORIZONTAL]->SetPosition(fp.X(), fp.Y() + Height() - _scrollbars[HORIZONTAL]->Height());
				_scrollbars[HORIZONTAL]->SetVisible(hscroll_visible);
			}

		}
		Scrollbar* Panel::HorizontalScrollbar() {

			return _scrollbars[HORIZONTAL];

		}
		Scrollbar* Panel::VerticalScrollbar() {

			return _scrollbars[VERTICAL];

		}
		SizeF Panel::RecalculateVisibleRegion() {

			float width = Width();
			float height = Height();

			if (VerticalScrollbar() && VerticalScrollbar()->Visible())
				width -= VerticalScrollbar()->Width();

			if (HorizontalScrollbar() && HorizontalScrollbar()->Visible())
				height -= HorizontalScrollbar()->Height();

			return SizeF(width, height);

		}
		RectangleF Panel::GetControlBounds() {

			float x1 = 0.0f;
			float y1 = 0.0f;
			float x2 = 0.0f;
			float y2 = 0.0f;

			for (auto i = Controls()->ControlsBegin(); i != Controls()->ControlsEnd(); ++i) {

				ControlPtr& control = *i;

				// Get the left offset. If the control is left-anchored, this is not important.
				if (!(control->Anchors() & ANCHOR_LEFT) && control->X() < x1)
					x1 = control->X();

				// Get the upper offset. If the control is top-anchored, this is not important.
				if (!(control->Anchors() & ANCHOR_TOP) && control->Y() < y1)
					y1 = control->Y();

				// Get the right offset. If the control is right-anchored, this is not important.
				if (!(control->Anchors() & ANCHOR_RIGHT) && control->X() + control->Width() > x2)
					x2 = control->X() + control->Width();

				// Get the lower offset. If the control is bottom-anchored, this is not important.
				if (!(control->Anchors() & ANCHOR_BOTTOM) && control->Y() + control->Height() > y2)
					y2 = control->Y() + control->Height();

			}

			// Return the rectangle representing the new area.
			return RectangleF(x1, y1, x2, y2);

		}

	}

}