#include "GuiManager.h"
#include "ControlController.h"
#include "io/Mouse.h"
#include "io/Keyboard.h"
#include <utility>

namespace Gui {

	GuiManager::GuiManager() : _last_mouse_position(Mouse::X, Mouse::Y) {

		_held_control = nullptr;
		_hovered_control = nullptr;
		_resort_needed = false;

		_keyboard_events_enabled = true;
		_mouse_events_enabled = true;

		_gui_scale = 1.0f;

	}
	GuiManager::~GuiManager() {}

	void GuiManager::AddControl(control_type& control) {

		// Set the control's parent manager to this object.
		ControlController(*control).SetManager(*this);

		// Add the control to our list.
		_controls.push_back(std::move(control));

		// Sort the list of controls by their Z depth.
		// Controls with smaller values of Z are drawn on top of those with larger values.
		Sort();

	}
	void GuiManager::RemoveControl(Control* control) {

		// Find the control in the list.
		auto iter = FindControlByAddress(control);

		// If it was found, remove it.
		if (iter != _controls.end())
			_controls.erase(iter);

	}
	Control* GuiManager::ActiveControl() {

		return _hovered_control;

	}
	Control* GuiManager::ControlAt(size_t index) {

		return std::next(_controls.begin(), index)->get();

	}
	size_t GuiManager::ControlCount() const {

		return _controls.size();

	}
	void GuiManager::Clear() {

		_controls.clear();

	}

	float GuiManager::Scale() const {

		return _gui_scale;

	}
	void GuiManager::SetScale(float scale) {

		_gui_scale = scale;

	}

	GuiStyleManager* GuiManager::StyleManager() {

		return &_style_manager;

	}

	void GuiManager::Update(UpdateEventArgs& e) {

		// Get mouse state.
		bool mouse_is_held = Mouse::ButtonDown(MB_LEFT | MB_RIGHT | MB_MIDDLE);
		bool mouse_is_pressed = Mouse::ButtonPressed(MB_LEFT | MB_RIGHT | MB_MIDDLE);
		bool mouse_is_dbl_pressed = Mouse::ButtonDoubleClicked(MB_LEFT | MB_RIGHT | MB_MIDDLE);

		// Reset the hovered Control pointer.
		Control* __prev_hovered_control = _hovered_control;
		_hovered_control = nullptr;
		bool __nothing_held = (!mouse_is_pressed && mouse_is_held && !_held_control);

		// Iterate through all of the Controls (lowest Z-depth to highest).
		for (auto it = _controls.begin(); it != _controls.end(); ++it) {

			// Get a pointer to the Control.
			Control* c = (*it).get();
			ControlController controller(*c);
			Point p = controller.GetFixedPosition();

			bool mouse_on = Mouse::InRegion(p.X(), p.Y(), p.X() + c->Width(), p.Y() + c->Height());

			if ( // Only handle the following events if...
				_mouse_events_enabled && // Mouse events are enabled.
				(mouse_on) // The mouse is currently on the Control.
				&& ((!_held_control && !__nothing_held) || _held_control == c) // No Control is held, or the current Control is held.
				&& (!_hovered_control) // No Control is assigned as the hovered Control for this iteration.
				) {

				// Assign the current Control as the hovered Control.
				_hovered_control = c;

				// If this Control is different from the one the mouse was previously on, handle OnMouseEnter event.
				if (_hovered_control != __prev_hovered_control)
					c->OnMouseEnter();

				// Handle OnMouseDown/OnMouseUp event.
				if (mouse_is_pressed) {
					if (_held_control != c)
						c->OnMouseDown();
					if (mouse_is_dbl_pressed)
						c->OnDoubleClick();
					_held_control = c;
					c->Focus();
				}

				// Handle OnMouseHover.
				c->OnMouseHover();

				// Handle OnMouseMove event.
				if (Mouse::X != _last_mouse_position.X() || Mouse::Y != _last_mouse_position.Y()) {
					_last_mouse_position.SetXY(Mouse::X, Mouse::Y);
					c->OnMouseMove();
				}

			}

			// If the Control was hovered previously but isn't anymore, Handle OnMouseLeave event.
			else if (__prev_hovered_control == c && _hovered_control != c)
				c->OnMouseLeave();

			// If the mouse isn't currrently being held, but this Control is assigned as the held Control, unassign it, and handle OnClick and OnMouseUp events.
			if (!mouse_is_held && _held_control == c) {
				if (_hovered_control == c) {
					c->OnClick();
					c->OnMouseUp();
				}
				_held_control = nullptr;
			}

			// Handle OnMove event.
			if (_mouse_events_enabled && (controller.PreviousPosition().X() != c->X() || controller.PreviousPosition().Y() || c->Y())) {
				c->OnMove();
				controller.SetPreviousPosition(c->X(), c->Y());
			}

			// Handle OnGotFocus/OnLostFocus events.
			if (c->HasFocus() && !controller.PrevFocus()) {
				controller.SetPrevFocus(true);
				c->OnGotFocus();
			}
			else if (!c->HasFocus() && controller.PrevFocus()) {
				controller.SetPrevFocus(false);
				c->OnLostFocus();
			}

			// Handle keyboard events.
			if (_keyboard_events_enabled) {
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
		if (_resort_needed) {
			Sort();
			_resort_needed = false;
		}

	}
	void GuiManager::Draw(DrawEventArgs& e) {

		// Save the state of the Graphics object.
		Drawing::Transform original_transform(e.Graphics().GetTransform());

		// Create a new transform that will be used for transforming each control.
		Drawing::Transform transform;

		for (auto it = _controls.rbegin(); it != _controls.rend(); ++it) {

			// Get a reference to the Control.
			Control& control = *(*it);

			// If the Control is not visible, do not draw it.
			if (!control.Visible())
				continue;

			//// Translate drawing so that the Control can draw itself at (0, 0).
			//transform.Reset();
			//transform.Translate(control.X(), control.Y());
			//transform.Compose(original_transform);
			//e.Graphics().SetTransform(transform);

			// Draw the Control.
			(*it)->Draw(e);

		}

		// Restore the previous transform.
		e.Graphics().SetTransform(original_transform);

	}
	void  GuiManager::BringToFront(Control* control) {

		// If this Control is the highest Control, do nothing.
		if (_controls.front().get() == control) return;

		// Set the Control's Z value equal to that of the highest Control.
		control->Z = _controls.front()->Z;

		// Find the Control in the list.
		auto iter = FindControlByAddress(control);

		// Return (silently) if the Control doesn't exist in the list.
		if (iter == _controls.end()) return;

		// Reposition the Control.
		_controls.splice(_controls.begin(), _controls, iter);


	}
	void  GuiManager::SendToBack(Control* control) {

		//// If this Control is the highest Control, do nothing.
		//if (_controls.back().get() == control) return;

		//// Set the Control's Z value equal to that of the lowest Control.
		//control->Z = _controls.back()->Z;

		//// Find the Control in the list.
		//auto iter = std::find(_controls.begin(), _controls.end(), control);

		//// Return (silently) if the Control doesn't exist in the list.
		//if (iter == _controls.end()) return;

		//// Reposition the Control.
		//_controls.splice(_controls.end(), _controls, iter);


	}
	void GuiManager::SetMouseEventsEnabled(bool value) {

		_mouse_events_enabled = value;

	}
	void GuiManager::SetkeyboardEventsEnabled(bool value) {

		_keyboard_events_enabled = value;

	}

	// Protected members
	std::list<GuiManager::control_type>::iterator GuiManager::FindControlByAddress(Control* ptr) {

		return std::find_if(_controls.begin(), _controls.end(), [&](control_type& p) { return p.get() == ptr; });
		
	}

	// Private members

	void GuiManager::Sort() {

		//_controls.sort([](Control* a, Control* b) {return a->Z < b->Z; });

	}

}