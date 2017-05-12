#pragma once
#include "gui/GuiManager.h"
#include "gui/IContainer.h"
#include "gui/IScrollable.h"
#include "gui/Control.h"
#include "Graphics.h"
#include "Exception.h"

namespace Gui {

	class Panel : public Control, public IContainer {

	public:
		Panel(float x, float y, float width, float height)
			: Control(Point(x, y), Size(width, height)),
			IContainer(this),
			_prev_size(width, height) {
		}

		virtual void Invalidate() override {

			//Controls()->InvalidateAll();


			Control::Invalidate();

		}

		virtual void OnResize() override {

			// Calculate the difference in size.
			float width_diff = Width() - _prev_size.Width();
			float height_diff = Height() - _prev_size.Height();

			// Reposition all anchored Controls.
			for (auto it = Controls()->_controls.rbegin(); it != Controls()->_controls.rend(); ++it) {

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
		
			e.Graphics().Clear(BackColor());

			//e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());

			Controls()->Draw(e);

			//// Clear main Bitmap.
			//Graphics::DrawClear(BackColor());

			//// Draw child to a separate Bitmap (to allow for relative positioning).
			//if (!__child_bitmap) __child_bitmap = al_create_bitmap(Width(), Height());
			//Graphics::SetDrawingTarget(__child_bitmap);
			//__manager.Draw();
			//Graphics::ResetDrawingTarget();

			//// Draw child Controls Bitmap.
			//al_draw_bitmap(__child_bitmap, X(), Y(), NULL);

		}
		virtual void Update(UpdateEventArgs& e) override {

			// Update child Controls.
			Controls()->Update(e);


		}

	private:
		Size _prev_size;
		
	};

}