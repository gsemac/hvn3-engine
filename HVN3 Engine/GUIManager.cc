#include <limits>
#include "GUIManager.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace GUI {

	GuiManager::GuiManager() : __mouse_last_pos(Mouse::X, Mouse::Y) {

		__held_control = nullptr;
		__hovered_control = nullptr;
		__resort_needed = false;

		__keyboard_events_enabled = true;
		__mouse_events_enabled = true;

	}

	void GuiManager::AddControl(Control* control) {

		// Add the new Control.
		control->__manager = this;
		__controls.push_back(control);

		// Sort the list of Controls by their Z value (larger Z => drawn earlier).
		Sort();

	}
	void GuiManager::RemoveControl(Control*& control) {
		if (!control) return;

		// Find the element in the list.
		auto iter = std::find(__controls.begin(), __controls.end(), control);

		// Remove the element if it was found.
		if (iter != __controls.end())
			__controls.erase(iter);

		// Free the memory used by object.
		delete control;
		control = nullptr;

	}
	Control* GuiManager::ActiveControl() {

		return __hovered_control;

	}

	float GuiManager::Scale() {

		return __gui_scale;

	}
	void GuiManager::SetScale(float scale) {

		__gui_scale = scale;

	}

	void GuiManager::Update(UpdateEventArgs& e) {

		// Get mouse state.
		bool mouse_is_held = Mouse::ButtonDown(MB_LEFT | MB_RIGHT | MB_MIDDLE);
		bool mouse_is_pressed = Mouse::ButtonPressed(MB_LEFT | MB_RIGHT | MB_MIDDLE);
		bool mouse_is_dbl_pressed = Mouse::ButtonDoubleClicked(MB_LEFT | MB_RIGHT | MB_MIDDLE);

		// Reset the hovered Control pointer.
		Control* __prev_hovered_control = __hovered_control;
		__hovered_control = nullptr;
		bool __nothing_held = (!mouse_is_pressed && mouse_is_held && !__held_control);

		// Iterate through all of the Controls (lowest Z-depth to highest).
		for (auto it = __controls.begin(); it != __controls.end(); ++it) {

			// Get a pointer to the Control.
			Control* c = *it;
			Point p = c->GetFixedPosition();

			bool mouse_on = Mouse::InRegion(p.X(), p.Y(), p.X() + c->Width(), p.Y() + c->Height());

			if ( // Only handle the following events if...
				__mouse_events_enabled && // Mouse events are enabled.
				(mouse_on) // The mouse is currently on the Control.
				&& ((!__held_control && !__nothing_held) || __held_control == c) // No Control is held, or the current Control is held.
				&& (!__hovered_control) // No Control is assigned as the hovered Control for this iteration.
				) {

				// Assign the current Control as the hovered Control.
				__hovered_control = c;

				// If this Control is different from the one the mouse was previously on, handle OnMouseEnter event.
				if (__hovered_control != __prev_hovered_control)
					c->OnMouseEnter();

				// Handle OnMouseDown/OnMouseUp event.
				if (mouse_is_pressed) {
					if (__held_control != c)
						c->OnMouseDown();
					if (mouse_is_dbl_pressed)
						c->OnDoubleClick();
					__held_control = c;
				}

				// Handle OnMouseHover.
				c->OnMouseHover();

				// Handle OnMouseMove event.
				if (Mouse::X != __mouse_last_pos.X() || Mouse::Y != __mouse_last_pos.Y()) {
					__mouse_last_pos.SetXY(Mouse::X, Mouse::Y);
					c->OnMouseMove();
				}

			}

			// If the Control was hovered previously but isn't anymore, Handle OnMouseLeave event.
			else if (__prev_hovered_control == c && __hovered_control != c)
				c->OnMouseLeave();

			// If the mouse isn't currrently being held, but this Control is assigned as the held Control, unassign it, and handle OnClick and OnMouseUp events.
			if (!mouse_is_held && __held_control == c) {
				if (__hovered_control == c) {
					c->OnClick();
					c->OnMouseUp();
				}
				__held_control = nullptr;
			}

			// Handle OnMove event.
			if (__mouse_events_enabled && (c->__previous_pos.X() != c->X() || c->__previous_pos.Y() || c->Y())) {
				c->OnMove();
				c->__previous_pos.SetXY(c->X(), c->Y());
			}

			// Handle OnGotFocus/OnLostFocus events.
			if (c->HasFocus() && !c->__prev_focus) {
				c->__prev_focus = true;
				c->OnGotFocus();
			}
			else if (!c->HasFocus() && c->__prev_focus) {
				c->__prev_focus = false;
				c->OnLostFocus();
			}

			// Handle keyboard events.
			if (__keyboard_events_enabled) {
				if (Keyboard::KeyPressed(KEY_ANY))
					c->OnKeyPressed();
				if (Keyboard::KeyReleased(KEY_ANY))
					c->OnKeyReleased();
				if (Keyboard::KeyDown(KEY_ANY))
					c->OnKeyDown();
			}

			// Handle Update event.
			c->Update(e);

		}

		// Sort if a resort is pending.
		if (__resort_needed) {
			Sort();
			__resort_needed = false;
		}

	}
	void GuiManager::Draw(DrawEventArgs& e) {

		for (auto it = __controls.rbegin(); it != __controls.rend(); ++it) {

			// If the Control is not visible, do not draw it.
			if (!(*it)->Visible())
				continue;

			// Draw the Control.
			(*it)->Draw(e);

		}

	}
	void  GuiManager::BringToFront(Control* control) {

		// If this Control is the highest Control, do nothing.
		if (__controls.front() == control) return;

		// Set the Control's Z value equal to that of the highest Control.
		control->Z = __controls.front()->Z;

		// Find the Control in the list.
		auto iter = std::find(__controls.begin(), __controls.end(), control);

		// Return (silently) if the Control doesn't exist in the list.
		if (iter == __controls.end()) return;

		// Reposition the Control.
		__controls.splice(__controls.begin(), __controls, iter);


	}
	void  GuiManager::SendToBack(Control* control) {

		// If this Control is the highest Control, do nothing.
		if (__controls.back() == control) return;

		// Set the Control's Z value equal to that of the lowest Control.
		control->Z = __controls.back()->Z;

		// Find the Control in the list.
		auto iter = std::find(__controls.begin(), __controls.end(), control);

		// Return (silently) if the Control doesn't exist in the list.
		if (iter == __controls.end()) return;

		// Reposition the Control.
		__controls.splice(__controls.end(), __controls, iter);


	}
	std::list<Control*>& GuiManager::Controls() {

		return __controls;

	}
	void GuiManager::SetMouseEventsEnabled(bool value) {

		__mouse_events_enabled = value;

	}
	void GuiManager::SetkeyboardEventsEnabled(bool value) {

		__keyboard_events_enabled = value;

	}

	// Private members

	void GuiManager::Sort() {

		__controls.sort([](Control* a, Control* b) {return a->Z < b->Z; });

	}

}