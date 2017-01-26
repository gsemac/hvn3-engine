//#ifndef __GUIPANEL_H
//#define __GUIPANEL_H
//#include "GUIManager.h"
//#include "Control.h"
//#include "Graphics.h"
//
//namespace Gui {
//
//	class Panel : public Control {
//
//	private:
//		Size __prev_size;
//		GuiManager __manager;
//		ALLEGRO_BITMAP* __child_bitmap;
//
//	public:
//		Panel(float x, float y, float width, float height) : Control(Point(x, y), Size(width, height)), __prev_size(width, height) {
//
//			__child_bitmap = nullptr;
//
//		}
//
//		void AddControl(Control* control) {
//
//			control->SetParent(this);
//			__manager.AddControl(control);
//
//		}
//		std::list<Control*>& Controls() {
//
//			return __manager.Controls();
//
//		}
//		void RemoveControl(Control* control) {
//
//			__manager.RemoveControl(control);
//
//		}
//		GuiManager* ChildManager() {
//
//			return &__manager;
//
//		}
//
//		virtual void Invalidate() override {
//
//			Control::Invalidate();
//
//		}
//
//		virtual void OnResize() override {
//
//			// Calculate the difference in size.
//			float width_diff = Width() - __prev_size.Width();
//			float height_diff = Height() - __prev_size.Height();
//
//			// Reposition all anchored Controls.
//			for (auto it = Controls().rbegin(); it != Controls().rend(); ++it) {
//
//				Control* c = *it;
//
//				if (c->Anchors() & ANCHOR_RIGHT) {
//					if (c->Anchors() & ANCHOR_LEFT)
//						c->Resize(c->Width() + width_diff, c->Height());
//					else
//						c->TranslateX(width_diff);
//				}
//
//				if (c->Anchors() & ANCHOR_BOTTOM) {
//					if (c->Anchors() & ANCHOR_TOP)
//						c->Resize(c->Width(), c->Height() + height_diff);
//					else
//						c->TranslateY(height_diff);
//				}
//
//				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_RIGHT) && !(c->Anchors() & ANCHOR_LEFT)))
//					c->TranslateX(width_diff / 2.0f);
//
//				if (c->Anchors() == ANCHOR_NONE || (!(c->Anchors() & ANCHOR_TOP) && !(c->Anchors() & ANCHOR_BOTTOM)))
//					c->TranslateY(height_diff / 2.0f);
//
//			}
//
//			// Update previous size/position.
//			__prev_size.Resize(Width(), Height());
//
//		}
//		virtual void OnPaint() override {
//
//			// Clear main Bitmap.
//			Graphics::DrawClear(BackColor());
//
//			// Draw child to a separate Bitmap (to allow for relative positioning).
//			if (!__child_bitmap) __child_bitmap = al_create_bitmap(Width(), Height());
//			Graphics::SetDrawingTarget(__child_bitmap);
//			__manager.Draw();
//			Graphics::ResetDrawingTarget();
//
//			// Draw child Controls Bitmap.
//			al_draw_bitmap(__child_bitmap, X(), Y(), NULL);
//
//		}
//
//		virtual void Update() {
//
//			// Update child Controls.
//			__manager.Update();
//
//		}
//
//	};
//
//}
//
//#endif