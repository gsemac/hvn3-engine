#include "hvn3/gui/ControlManager.h"
#include "hvn3/gui/ControlController.h"
#include "hvn3/gui/_GuiManager.h"
#include "hvn3/gui/Control.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/io/Keyboard.h"
#include "hvn3/exceptions/Exception.h"

namespace hvn3 {

	namespace Gui {

		ControlManager::ControlManager(Gui::GuiManager* gui_manager, const RectangleF& dockable_region) :
			_last_mouse_position(Mouse::X, Mouse::Y),
			_gui_manager(gui_manager),
			_dockable_region(dockable_region),
			_temp_dockable_region(dockable_region) {

			_held_control = nullptr;
			_hovered_control = nullptr;
			_resort_needed = false;

			_keyboard_events_enabled = true;
			_mouse_events_enabled = true;

		}
		ControlManager::~ControlManager() {}

		Handle<Control> ControlManager::AddControl(content_type& control) {

			// Set the control's parent manager to this object.
			System::ControlController(*control).SetManager(_gui_manager);

			// If the control is docked, add it to the docked controls list.
			if (control->Dock() != DockStyle::None)
				_addDockedControl(control.get());

			// Add the control to our collection.
			_controls.push_back(std::move(control));

			// Re-sort the collection of controls.
			Sort();

			// Return a handle to the control that was just added.
			return _controls.back().get();

		}
		bool ControlManager::RemoveControl(const Handle<Control>& control) {

			// Find the control in the list.
			auto iter = FindControlByAddress(control);

			if (iter == _controls.end())
				return false;

			// Remove it from the docked controls list if present.
			//if ((*iter)->Dock() != DockStyle::None)
			//	_controls.erase();

			// Add it to the pending removal list.
			_pendControlForRemoval(iter);

			// Return true to indicate that the control was successfully marked for removal.
			return true;

		}
		void ControlManager::MoveControl(const Handle<Control>& control, ControlManager* other) {

			// Get a pointer to the control.
			Control* ptr = control;

			// Find the control in our collection.
			auto iter = FindControlByAddress(ptr);

			// Move it into a new smart pointer object, invalidating the old one.
			ControlPtr new_ptr = std::move(*iter);

			// Mark the old control for removal.
			_pendControlForRemoval(iter);

			// Add the control to the other manager.
			other->AddControl(new_ptr);

		}
		void ControlManager::MoveControl(const Handle<Control>& control, ControlPtr& ptr) {

			// Find the control in our collection.
			auto iter = FindControlByAddress(control);

			// If the control could not be found, do nothing.
			if (iter == _controls.end())
				return;

			// Move it into the new smart pointer object, invalidating the old one.
			ptr = std::move(*iter);

			// Mark the old control for removal.
			_pendControlForRemoval(iter);

		}
		Handle<Control> ControlManager::ActiveControl() {

			return _hovered_control;

		}
		const Handle<Control> ControlManager::ActiveControl() const {

			return _hovered_control;

		}
		void ControlManager::ClearActiveControl() {

			if (_hovered_control != nullptr)
				_hovered_control->OnMouseLeave();

			_hovered_control = nullptr;

		}
		Handle<Control> ControlManager::ControlAt(size_t index) {

			return std::next(_controls.begin(), index)->get();

		}
		size_t ControlManager::ControlCount() const {

			return _controls.size();

		}
		void ControlManager::Clear() {

			_controls.clear();

		}

		ControlManager::collection_type::iterator ControlManager::ControlsBegin() {

			return _controls.begin();

		}
		ControlManager::collection_type::iterator ControlManager::ControlsEnd() {

			return _controls.end();

		}
		ControlManager::collection_type::const_iterator ControlManager::ControlsBegin() const {

			return _controls.begin();

		}
		ControlManager::collection_type::const_iterator ControlManager::ControlsEnd() const {

			return _controls.end();

		}

		void ControlManager::OnUpdate(UpdateEventArgs& e) {

			// Get mouse state.
			bool mouse_is_held = Mouse::ButtonDown(MouseButton::Left | MouseButton::Right | MouseButton::Middle);
			bool mouse_is_pressed = Mouse::ButtonPressed(MouseButton::Left | MouseButton::Right | MouseButton::Middle);
			bool mouse_is_dbl_pressed = Mouse::ButtonDoubleClicked(MouseButton::Left | MouseButton::Right | MouseButton::Middle);

			// Reset the hovered Control pointer.
			Control* __prev_hovered_control = _hovered_control;
			_hovered_control = nullptr;
			bool __nothing_held = (!mouse_is_pressed && mouse_is_held && !_held_control);

			// Iterate through all of the Controls (lowest Z-depth to highest).
			for (auto it = _controls.begin(); it != _controls.end(); ++it) {

				// Get a pointer to the Control.
				Control* c = (*it).get();

				// If the control is disabled, ignore it.
				if (!c->Enabled())
					continue;

				System::ControlController controller(*c);
				PointF p = controller.GetFixedPosition();

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
							c->OnMouseDown(MouseEventArgs(_getMousePositionRelativeToControl(c)));
						if (mouse_is_dbl_pressed)
							c->OnDoubleClick();
						_held_control = c;
						c->Focus();
					}

					// Handle OnMouseHover.
					c->OnMouseHover();

					// Handle OnMouseMove event.
					if (Mouse::X != _last_mouse_position.X() || Mouse::Y != _last_mouse_position.Y()) {
						_last_mouse_position = PointF(Mouse::X, Mouse::Y);
						c->OnMouseMove(MouseMoveEventArgs(_getMousePositionRelativeToControl(c)));
					}

				}

				// If the Control was hovered previously but isn't anymore, Handle OnMouseLeave event.
				else if (__prev_hovered_control == c && _hovered_control != c)
					c->OnMouseLeave();

				// If the mouse isn't currrently being held, but this Control is assigned as the held Control, unassign it, and handle OnClick and OnMouseUp events.
				if (!mouse_is_held && _held_control == c) {
					if (_hovered_control == c) {
						c->OnClick();
						c->OnMouseUp(MouseEventArgs(_getMousePositionRelativeToControl(c)));
					}
					_held_control = nullptr;
				}

				// Handle OnMove event.
				if (_mouse_events_enabled && (controller.PreviousPosition().X() != c->X() || controller.PreviousPosition().Y() || c->Y())) {
					c->OnMove(MoveEventArgs(controller.PreviousPosition()));
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
					if (Keyboard::KeyPressed(Key::Any))
						c->OnKeyPressed();
					if (Keyboard::KeyReleased(Key::Any))
						c->OnKeyReleased();
					if (Keyboard::KeyDown(Key::Any))
						c->OnKeyDown();
				}

				// Handle Update event.
				c->OnUpdate(e);

			}

			_applyDockStyleForAllControls();

			// Remove all controls that are currently pending removal.
			_eraseAllControlsPendingRemoval();

			// Sort if a resort is pending.
			if (_resort_needed) {
				Sort();
				_resort_needed = false;
			}

		}
		void ControlManager::OnDraw(DrawEventArgs& e) {

			// Save the state of the Graphics object.
			Graphics::Transform original_transform(e.Graphics().GetTransform());

			// Create a new transform that will be used for transforming each control.
			Graphics::Transform transform;
			transform.Translate(_control_offset.X(), _control_offset.Y());
			transform.Compose(original_transform);
			e.Graphics().SetTransform(transform);

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
				(*it)->OnDraw(e);

			}

			// Restore the previous transform.
			e.Graphics().SetTransform(original_transform);

		}
		void ControlManager::BringToFront(const Handle<Control>& control) {

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
		void ControlManager::SendToBack(const Handle<Control>& control) {

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
		void ControlManager::SetControlOffset(float x_offset, float y_offset) {

			_control_offset = PointF(x_offset, y_offset);

		}
		const PointF& ControlManager::ControlOffset() const {

			return _control_offset;

		}
		void ControlManager::SetMouseEventsEnabled(bool value) {

			_mouse_events_enabled = value;

		}
		void ControlManager::SetkeyboardEventsEnabled(bool value) {

			_keyboard_events_enabled = value;

		}
		void ControlManager::InvalidateAll() {

			for (auto it = _controls.rbegin(); it != _controls.rend(); ++it)
				it->get()->Invalidate();

		}

		const RectangleF& ControlManager::DockableRegion() const {

			return _temp_dockable_region;

		}
		void ControlManager::SetDockableRegion(const RectangleF& region) {

			_dockable_region = region;

		}
		void ControlManager::ResizeDockableRegion(const RectangleF& region) {

			_temp_dockable_region = region;

		}
		void ControlManager::ResetDockableRegion() {

			_temp_dockable_region = _dockable_region;

		}

		// Protected members
		ControlManager::collection_type::iterator ControlManager::FindControlByAddress(Control* ptr) {

			return std::find_if(_controls.begin(), _controls.end(), [&](ControlPtr& p) { return p.get() == ptr; });

		}

		// Private members
		void ControlManager::_addDockedControl(Control* control) {

			_docked_controls.push_back(control);

		}
		void ControlManager::_removeDockedControl(Control* control) {

			// Find the control in the collection.
			auto it = std::find(_docked_controls.begin(), _docked_controls.end(), control);

			// If the control does not exist in the collection, do nothing.
			if (it == _docked_controls.end())
				return;

			// Remove the control from the collection.
			_docked_controls.erase(it);

		}
		void ControlManager::_applyDockStyleForAllControls() {

			// Reset the dockable region; it will updated as docking is applied to each control.
			ResetDockableRegion();

			for (auto it = _docked_controls.begin(); it != _docked_controls.end(); ++it)
				_applyDockStyleToControl(*it);

		}
		void ControlManager::_pendControlForRemoval(collection_type::iterator control_it) {

			// Add the control to the list of pending removals.
			_pending_removal.push_back(control_it);

			// If the control is in the docked control collection, remove it from there immediately.
			_removeDockedControl((*control_it).get());

		}
		void ControlManager::_eraseAllControlsPendingRemoval() {

			// Remove all controls in the list of pending removals.
			for (size_t i = 0; i < _pending_removal.size(); ++i)
				_controls.erase(_pending_removal[i]);

			// Clear the list of pending removals.
			_pending_removal.clear();

		}

		void ControlManager::_applyDockStyleToControl(Control* c) {

			switch (c->Dock()) {

			case DockStyle::Top:

				c->SetPosition(DockableRegion().X(), DockableRegion().Y());
				c->SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP);
				c->SetWidth(DockableRegion().Width());

				ResizeDockableRegion(RectangleF::Crop(DockableRegion(), CropSide::Top, c->Height()));

				break;

			case DockStyle::Left:

				c->SetPosition(DockableRegion().X(), DockableRegion().Y());
				c->SetAnchors(ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_BOTTOM);
				c->SetHeight(DockableRegion().Height());

				ResizeDockableRegion(RectangleF::Crop(DockableRegion(), CropSide::Left, c->Width()));

				break;

			case DockStyle::Right:

				c->SetPosition((DockableRegion().X() + DockableRegion().Width()) - c->Width(), DockableRegion().Y());
				c->SetAnchors(ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_BOTTOM);
				c->SetHeight(DockableRegion().Height());

				ResizeDockableRegion(RectangleF::Crop(DockableRegion(), CropSide::Right, c->Width()));

				break;

			case DockStyle::Bottom:

				c->SetPosition(DockableRegion().X(), (DockableRegion().Y() + DockableRegion().Height()) - c->Height());
				c->SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);
				c->SetWidth(DockableRegion().Width());

				ResizeDockableRegion(RectangleF::Crop(DockableRegion(), CropSide::Bottom, c->Height()));

				break;

			case DockStyle::Fill:

				c->SetPosition(DockableRegion().X(), DockableRegion().Y());
				c->SetAnchors(ANCHOR_ALL);
				c->SetWidth(DockableRegion().Width());
				c->SetHeight(DockableRegion().Height());

				break;


			}

		}
		PointF ControlManager::_getMousePositionRelativeToControl(Control* c) const {

			return Mouse::Position() - c->FixedPosition();

		}

		void ControlManager::Sort() {

			// Controls with smaller values of Z are drawn on top of those with larger values.

			//_controls.sort([](Control* a, Control* b) {return a->Z < b->Z; });

		}

	}

}