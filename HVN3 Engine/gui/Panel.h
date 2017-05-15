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
		Panel(float x, float y, float width, float height)
			: Control(Point(x, y), Size(width, height)),
			IContainer(this),
			IScrollable(this, Rectangle(width, 500)),
			_prev_size(width, height) {
		
			_scrollbars[0] = nullptr;
			_scrollbars[1] = nullptr;

		}

		virtual void Invalidate() override {

			//Controls()->InvalidateAll();


			Control::Invalidate();

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
		virtual void OnResize() override {

			// Calculate the difference in size.
			float width_diff = Width() - _prev_size.Width();
			float height_diff = Height() - _prev_size.Height();

			// Reposition all anchored Controls.
			for (auto it = Controls()->ControlsEnd(); it-- != Controls()->ControlsBegin();) {

				Control* c = it->get();

				if (c->Anchors() & ANCHOR_RIGHT) {
					if (c->Anchors() & ANCHOR_LEFT)
						c->Resize(c->Width() + width_diff, c->Height());
					else
						c->TranslateX(width_diff);
				}

				if (c->Anchors() & ANCHOR_BOTTOM) {
					if (c->Anchors() & ANCHOR_TOP)
						c->Resize(c->Width(), c->Height() + height_diff);
					else
						c->TranslateY(height_diff);
				}

				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_RIGHT) && !(c->Anchors() & ANCHOR_LEFT)))
					c->TranslateX(width_diff / 2.0f);

				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_TOP) && !(c->Anchors() & ANCHOR_BOTTOM)))
					c->TranslateY(height_diff / 2.0f);

			}

			// Update previous size/position.
			_prev_size.Resize(Width(), Height());

		}
		virtual void OnPaint(PaintEventArgs& e) override {

			// Draw the background color of the control.
			e.Graphics().Clear(BackColor());

			// Draw child controls.
			Controls()->Draw(e);

		}
		virtual void OnMove(MoveEventArgs& e) override {

			// Update the position of the scrollbars to match the control's new position.
			if (_scrollbars[VERTICAL] != nullptr)
				_scrollbars[VERTICAL]->SetXY(X() + Width() - _scrollbars[VERTICAL]->Width(), Y());
			if (_scrollbars[HORIZONTAL] != nullptr)
				_scrollbars[HORIZONTAL]->SetXY(X(), Y() + Height() - _scrollbars[HORIZONTAL]->Height());

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

		}

	private:
		Scrollbar* _scrollbars[2];
		Size _prev_size;

	};

}