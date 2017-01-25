#ifndef __CONTAINER_H
#define __CONTAINER_H
#include <list>
#include "Control.h"
#include "GUIManager.h"

namespace Gui {
	// A Container is a Control that manages its own set of child Controls through a GuiManager object. 
	class Container : public Control {

	private:
		Size __prev_size;
		Point __prev_pos;
		GuiManager __manager;

		// Returns true if the Container is currrently handling events for one of its Controls.
		bool HasActiveChild() override;

	public:
		Container(float x, float y, float width, float height);

		// Adds a new Control to the Container.
		void AddControl(Control* control);

		// Removes the Control from the Container, frees it from memory, and nullifies the pointer.
		void RemoveControl(Control*& control);

		// Returns a reference to the list of Controls managed by the inner GuiManager object.
		std::list<Control*>& Controls();

		virtual void OnResize() override {

			// Calculate the difference in size.
			float width_diff = Width() - __prev_size.Width();
			float height_diff = Height() - __prev_size.Height();

			// Reposition all anchored Controls.
			for (auto it = Controls().rbegin(); it != Controls().rend(); ++it) {

				Control* c = *it;

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

				if (c->Anchors() == ANCHOR_NONE) {

					c->Translate(width_diff / 2.0f, height_diff / 2.0f);

				}

			}

			// Update previous size/position.
			__prev_size.Resize(Width(), Height());

		}
		virtual void OnPaint() override {

			// Draw child Controls.
			__manager.Draw();

		}
		virtual void Update() override {

			// Ipdate child Controls.
			__manager.Update();

		}
		virtual void OnMove() override {

			// Move all child Controls with the Container.
			for (auto it = Controls().rbegin(); it != Controls().rend(); ++it) {
				Control* c = *it;
				c->Translate(X() - __prev_pos.X(), Y() - __prev_pos.Y());
			}

			// Update last position.
			__prev_pos = Point(X(), Y());

		}

	};
}

#endif