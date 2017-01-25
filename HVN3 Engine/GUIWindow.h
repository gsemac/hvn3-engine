#ifndef __GUIWINDOW_H
#define __GUIWINDOW_H
#include <allegro5\allegro_primitives.h>
#include <vector>
#include "Bitmap.h"
#include "Container.h"
#include "GUITheme.h"
#include "GUIPanel.h"
#include "UTF8String.h"
#include "Mouse.h"
#include "Graphics.h"

namespace Gui {

	class Window : public Control {

	private:
		static int __titlebar_height, __resize_region_thickness;

		Utf8String* __text;
		Font* __font;
		Sprite* __exit_icon;

		Point __drag_offset;
		bool __dragging, __resizing;
		unsigned int __resizing_side;
		int __orig_height, __orig_width, __orig_x, __orig_y;
		float __height_diff, __width_diff;

		Panel __panel;
		Bitmap __child_bitmap;

		// Returns the resize regions that the mouse is currently in. Returns 0 if the mouse is not in a resize region.
		unsigned int GetMouseResizeRegions() {

			unsigned int side = 0;
			float scale = Scale();

			if (Mouse::InRegion(X(), Y() + Height() * scale - __resize_region_thickness * scale, X() + Width() * scale, Y() + Height() * scale))
				side |= BOTTOM;
			else if (Mouse::InRegion(X(), Y(), X() + Width() * scale, Y() + __resize_region_thickness * scale))
				side |= TOP;

			if (Mouse::InRegion(X(), Y(), X() + __resize_region_thickness * scale, Y() + Height() * scale))
				side |= LEFT;
			else if (Mouse::InRegion(X() + Width() * scale - __resize_region_thickness * scale, Y(), X() + Width() * scale, Y() + Height() * scale))
				side |= RIGHT;

			return side;

		}

		// Sets the cursor depending on where it is on the Window.
		void SetResizeCursor() {

			unsigned int region = GetMouseResizeRegions();

			if (!region)
				Mouse::SetCursor(SystemCursor::Default);
			if (region & TOP)
				Mouse::SetCursor(SystemCursor::ResizeN);
			else if (region & BOTTOM)
				Mouse::SetCursor(SystemCursor::ResizeS);
			if (region & LEFT)
				if (region & TOP)
					Mouse::SetCursor(SystemCursor::ResizeNW);
				else if (region & BOTTOM)
					Mouse::SetCursor(SystemCursor::ResizeSW);
				else
					Mouse::SetCursor(SystemCursor::ResizeW);
			else if (region & RIGHT)
				if (region & TOP)
					Mouse::SetCursor(SystemCursor::ResizeNE);
				else if (region & BOTTOM)
					Mouse::SetCursor(SystemCursor::ResizeSE);
				else
					Mouse::SetCursor(SystemCursor::ResizeE);

		}

		// Adjusts the size/position of the Window when resizing.
		void HandleResizing() {

			float new_width = Width();
			float new_height = Height();
			float scale = Scale();

			if (__resizing_side & BOTTOM) {

				// Calculate the differnce in height between the mouse's current position and it's new position.
				__height_diff = Mouse::Y - __drag_offset.Y();
	
				// If there is a difference, resize the Control.
				if (std::abs((__orig_height + __height_diff) - Height()) > 0.0f)
					new_height = __orig_height + (__height_diff / scale);

			}
			else if (__resizing_side & TOP) {

				// Calculate the differnce in height between the mouse's current position and it's new position.
				__height_diff = Mouse::Y - __drag_offset.Y();

				// If there is a difference, resize the Control.
				if (std::abs((__orig_height - __height_diff) - Height()) > 0.0f) {
					new_height = Clamp(__orig_height - (__height_diff / scale), MinimumSize().Height(), MaximumSize().Height());
					SetY(__orig_y + (__orig_height - new_height) * scale);
				}

			}
			if (__resizing_side & RIGHT) {

				// Calculate the difference in width between the mouse's current position and it's new position.
				__width_diff = Mouse::X - __drag_offset.X();

				// If there is a difference, resize the Control.
				if (std::abs((__orig_width + __width_diff) - Width()) > 0.0f)
					new_width = __orig_width + (__width_diff / scale);

			}
			else if (__resizing_side & LEFT) {

				// Calculate the difference in width between the mouse's current position and it's new position.
				__width_diff = Mouse::X - __drag_offset.X();

				// If there is a difference, resize the Control.
				if (std::abs((__orig_width - __width_diff) - Width()) > 0.0f) {
					new_width = Clamp(__orig_width - (__width_diff / scale), MinimumSize().Width(), MaximumSize().Width());
					SetX(__orig_x + (__orig_width - new_width) * scale);
				}

			}

			if (new_width != Width() || new_height != Height())
				Resize(std::round(new_width), std::round(new_height));

		}

		bool HasActiveChild() override {

			return __panel.ChildManager()->ActiveControl();

		}

	public:
		Window(float x, float y, float width, float height, const char* text) :
			Control(Point(x, y), Size(width, height + __titlebar_height)),
			__panel(0.0f, __titlebar_height, width, height),
			__drag_offset(0.0f, 0.0f),
			__child_bitmap(width, height)
		{

			// Set up titlebar.
			__font = ActiveTheme()->DefaultFont();
			__exit_icon = ActiveTheme()->ExitIcon();
			__text = new Utf8String(text);

			// Make sure the Window cannot be made smaller than its titlebar.
			SetMinimumSize(Size(__titlebar_height, __titlebar_height));

			// Set up Panel.
			__panel.SetParent(this);

			// Set up state/state tracking variables.
			__dragging = false;
			__resizing = false;
			__resizing_side = 0;
			__orig_width = Width();
			__orig_height = Height();
			__width_diff = 0;
			__height_diff = 0;

		}
		~Window() {

			// Delete text objext.
			if (__text) delete __text;

		}

		void AddControl(Control* control) {

			// Add the Control to the Panel.
			__panel.AddControl(control);

		}
		void RemoveControl(Control* control) {

			// Remove the Control from the Panel.
			__panel.RemoveControl(control);

		}
		std::list<Control*>& Controls() {

			// Return a reference to the Panel's Controls.
			return __panel.Controls();

		}

		void OnMouseDown() override {

			// Place the Window in front of all other Controls under the same manager.
			BringToFront();

			if (HasActiveChild()) return;

			if (!__dragging && !__resizing && GetMouseResizeRegions()) {

				// Initialize resizing variables.
				__resizing = true;
				__resizing_side = GetMouseResizeRegions();
				__drag_offset.SetXY(Mouse::X, Mouse::Y);
				__orig_width = Width();
				__orig_height = Height();
				__orig_x = X();
				__orig_y = Y();
				__width_diff = 0;
				__height_diff = 0;

			}
			else if (!__resizing && !__dragging) {

				// Initialize dragging variables.
				__dragging = true;
				__drag_offset.SetXY(X() - Mouse::X, Y() - Mouse::Y);

			}

		}
		void OnMouseUp() override {
			if (HasActiveChild()) return;

			// Stop dragging/resizing.
			__dragging = false;
			__resizing = false;

			// Reset the cursor to the default.
			Mouse::SetCursor(SystemCursor::Default);

		}
		void OnMouseMove() override {
			if (HasActiveChild()) return;

			// Set the cursor to a resize cursor if it passes over any edges.
			if (!__resizing && !__dragging) SetResizeCursor();

		}
		void OnMouseLeave() override {
			std::cout << "Window: OnMouseLeave()\n";
			// If the mouse leaves the Control and it's not resizing, reset the cursor.
			if (!__resizing) Mouse::SetCursor(SystemCursor::Default);

		}
		void OnResize() override {

			// Resize the Panel.
			__panel.Resize(Width(), Height() - __titlebar_height);

			// Resize child Bitmap.
			__child_bitmap = Bitmap(Width(), Height());

		}
		void OnPaint() override {

			// Draw titlebar.
			al_draw_filled_rectangle(X(), Y(), X() + Width(), Y() + __titlebar_height, BackColor()->AlPtr());
			al_draw_rectangle(X() + 1, Y() + 1, X() + Width(), Y() + __titlebar_height, al_map_rgb(17, 17, 17), 1.0f);

			// Draw titlebar text.
			if (__font)
				al_draw_shadow_ustr(__font->AlPtr(), al_map_rgb(186, 186, 186), Color::FromArgb(0, 0, 0, 0.5f).AlPtr(), (std::round)(X() + Width() / 2.0f),
				(std::round)(Y() + (__titlebar_height / 2.0f) - (__font->Height() / 2.0f)) - 1, ALLEGRO_ALIGN_CENTRE, __text->AlPtr());

			// Draw exit button.
			float exit_x = X() + Width() - __exit_icon->Width() - (__exit_icon->Width() / 2.0f);
			float exit_y = Y() + (__exit_icon->Height() / 2.0f) + 1.0f;
			al_draw_tinted_bitmap(__exit_icon->AlPtr(),
				Mouse::InRegion(exit_x, exit_y, exit_x + __exit_icon->Width(), exit_y + __exit_icon->Height()) ? al_map_rgba_f(0.5f, 0.5f, 0.5f, 1.0f) : al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f),
				exit_x, exit_y, NULL);

			// Draw main window area.
			al_draw_filled_rectangle(X(), Y() + __titlebar_height, X() + Width(), Y() + Height(), BackColor()->AlPtr());

			// Draw Panel's child Controls.
			Graphics::SetDrawingTarget(__child_bitmap.AlPtr());
			Graphics::DrawClear(Color::Transparent);
			__panel.ChildManager()->Draw();
			Graphics::ResetDrawingTarget();

			// Draw main window border.
			al_draw_rectangle(X() + 1, Y() + __titlebar_height, X() + Width(), Y() + Height(), al_map_rgb(17, 17, 17), 1.0f);

			// Draw child control surface.
			al_draw_bitmap(__child_bitmap.AlPtr(), X() + 1, Y() + __titlebar_height, NULL);

		}

		void Update() override {

			__panel.ChildManager()->SetMouseEventsEnabled((Manager()->ActiveControl() == this));
			__panel.Update();

			if (!Mouse::ButtonDown(MB_LEFT) && (__dragging || __resizing)) {
				__dragging = false;
				__resizing = false;
				Mouse::SetCursor(SystemCursor::Default);

			}

			if (__resizing)
				HandleResizing();
			else if (__dragging) {
				SetXY(__drag_offset.X() + Mouse::X, __drag_offset.Y() + Mouse::Y);
			}

		}

	};

}

#endif