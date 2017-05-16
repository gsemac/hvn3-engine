#pragma once
#include "gui/GuiManager.h"
#include "gui/IContainer.h"
#include "gui/IScrollable.h"
#include "gui/Control.h"
#include "gui/Scrollbar.h"
#include "Graphics.h"
#include "Exception.h"
#define PANEL_SCROLLBAR_DEFAULT_WIDTH 10

namespace Gui {

	class Panel : public Control, public IContainer, public IScrollable {

		enum SCROLLBAR : bool {
			VERTICAL,
			HORIZONTAL
		};

	public:
		Panel(const Point& position, const Size& dimensions) :
			Panel(position, dimensions, dimensions) {
		}
		Panel(const Point& position, const Size& dimensions, const Size& scrollable_region) :
			Control(position, dimensions),
			IContainer(this),
			IScrollable(this, scrollable_region),
			_prev_size(dimensions.Width(), dimensions.Height()) {

			_scrollbars[0] = nullptr;
			_scrollbars[1] = nullptr;

		}
		virtual ~Panel() {

			// Remove scrollbars from the manager.
			// This is important, because a panel could be removed from a control manager without destroying the control manager.

			if (!Manager() || !Manager()->ControlManager())
				return;

			if (_scrollbars[VERTICAL])
				Manager()->ControlManager()->RemoveControl(_scrollbars[VERTICAL]);

			if (_scrollbars[HORIZONTAL])
				Manager()->ControlManager()->RemoveControl(_scrollbars[HORIZONTAL]);

		}

		virtual void OnScroll(ScrollEventArgs& e) override {

			// For each child control, we want it to be shifted by the scroll offset.
			if (e.Orientation() == Orientation::Vertical)
				Controls()->SetControlOffset(Controls()->ControlOffset().X(), Round(-e.Position()));
			else
				Controls()->SetControlOffset(Round(-e.Position()), Controls()->ControlOffset().Y());

			IScrollable::OnScroll(e);

			Invalidate();

		}
		virtual void OnResize(ResizeEventArgs& e) override {

			// Update the visible region.
			SetVisibleRegion(e.NewSize());

			// Update the positions of anchored controls.
			UpdateAnchors(e);

			// Update the positions/sizes of the scrollbars.
			UpdateScrollbarPositionsAndSizes();
			
		}
		virtual void OnPaint(PaintEventArgs& e) override {

			// Draw the background color of the control.
			e.Graphics().Clear(BackColor());

			// Draw child controls.
			Controls()->Draw(e);

		}
		virtual void OnMove(MoveEventArgs& e) override {

			// Update the position of the scrollbars to match the control's new position.
			UpdateScrollbarPositionsAndSizes();

			Control::OnMove(e);

		}
		virtual void Update(UpdateEventArgs& e) override {

			// Update child Controls.
			if (IsActiveControl())
				Controls()->Update(e);
			else
				Controls()->ClearActiveControl();

		}
		virtual void OnManagerChanged(ManagerChangedEventArgs& e) override {

			// If the new manager is null, do nothing but set scrollbars to null. 
			// They would've been deleted by the previous manager's control manager.
			if (Manager() == nullptr) {
				_scrollbars[VERTICAL] = nullptr;
				_scrollbars[HORIZONTAL] = nullptr;
				return;
			}

			// If scrollbars haven't been created yet, create them.
			if (_scrollbars[VERTICAL] == nullptr)
				_scrollbars[VERTICAL] = new Scrollbar(this, Point(X() + Width() - PANEL_SCROLLBAR_DEFAULT_WIDTH, Y() + PANEL_SCROLLBAR_DEFAULT_WIDTH), Size(PANEL_SCROLLBAR_DEFAULT_WIDTH, Height() - PANEL_SCROLLBAR_DEFAULT_WIDTH), Orientation::Vertical);
			if (_scrollbars[HORIZONTAL] == nullptr)
				_scrollbars[HORIZONTAL] = new Scrollbar(this, Point(X(), Y() + Height() - PANEL_SCROLLBAR_DEFAULT_WIDTH), Size(Width() - PANEL_SCROLLBAR_DEFAULT_WIDTH, PANEL_SCROLLBAR_DEFAULT_WIDTH), Orientation::Horizontal);

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

	private:
		Scrollbar* _scrollbars[2];
		Size _prev_size;

		void UpdateScrollbarPositionsAndSizes() {

			Point fp = Point(X(), Y());//FixedPosition(); ? 

			if (_scrollbars[VERTICAL] != nullptr) {
				_scrollbars[VERTICAL]->Resize(PANEL_SCROLLBAR_DEFAULT_WIDTH, Height() - PANEL_SCROLLBAR_DEFAULT_WIDTH);
				_scrollbars[VERTICAL]->SetXY(fp.X() + Width() - _scrollbars[VERTICAL]->Width(), fp.Y());
				_scrollbars[VERTICAL]->SetVisible(VisibleRegion().Height() < ScrollableRegion().Height());
			}

			if (_scrollbars[HORIZONTAL] != nullptr) {
				_scrollbars[HORIZONTAL]->Resize(Width() - PANEL_SCROLLBAR_DEFAULT_WIDTH, PANEL_SCROLLBAR_DEFAULT_WIDTH);
				_scrollbars[HORIZONTAL]->SetXY(fp.X(), fp.Y() + Height() - _scrollbars[HORIZONTAL]->Height());
				_scrollbars[HORIZONTAL]->SetVisible(VisibleRegion().Width() < ScrollableRegion().Width());
			}

		}

	};

}