#pragma once
#include <list>
#include <vector>
#include <memory>
#include "IUpdatable.h"
#include "IDrawable.h"
#include "Point2d.h"
#include "Handle.h"
#include "gui/Control.h"

namespace hvn3 {

	namespace Gui {

		//class Control;
		class GuiManager;

		class ControlManager : public IUpdatable, public IDrawable {

			typedef ControlPtr content_type;
			typedef std::list<content_type> collection_type;

		public:
			ControlManager(GuiManager* gui_manager, const RectangleF& dockable_region);
			virtual ~ControlManager();

			// Adds a new control to the manager.
			virtual Handle<Control> AddControl(content_type& control);
			// Removes a control from the manager. Does nothing if the control does not exist.
			virtual bool RemoveControl(const Handle<Control>& control);
			// Moves a control from the current manager into an other manager.
			virtual void MoveControl(const Handle<Control>& control, ControlManager* other);
			virtual void MoveControl(const Handle<Control>& control, ControlPtr& ptr);
			Handle<Control> ActiveControl();
			const Handle<Control> ActiveControl() const;
			void ClearActiveControl();
			// Returns the control at the given index.
			Handle<Control> ControlAt(size_t index);
			// Returns the number of controls belonging to the manager.
			size_t ControlCount() const;
			void Clear();

			collection_type::iterator ControlsBegin();
			collection_type::iterator ControlsEnd();
			collection_type::const_iterator ControlsBegin() const;
			collection_type::const_iterator ControlsEnd() const;

			void BringToFront(const Handle<Control>& control);
			void SendToBack(const Handle<Control>& control);

			void SetControlOffset(float x_offset, float y_offset);
			const PointF& ControlOffset() const;
			void SetkeyboardEventsEnabled(bool value);
			void SetMouseEventsEnabled(bool value);
			void InvalidateAll();

			// Returns the region use for docking child controls.
			const RectangleF& DockableRegion() const;
			// Sets the region used for docking child controls.
			void SetDockableRegion(const RectangleF& region);
			// Temporarily sets a new dockable region.
			void ResizeDockableRegion(const RectangleF& region);
			// Restores the dockable region, undoing any changes made by ResizeDockableRegion.
			void ResetDockableRegion();

			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnDraw(DrawEventArgs& e) override;

		protected:
			collection_type::iterator FindControlByAddress(Control* ptr);
			// Resorts the Controls by their Z coordinate (largest first).
			void Sort();

		private:
			// Adds a control to the list of docked controls.
			void _addDockedControl(Control* control);
			// Removes a control from the list of docked controls.
			void _removeDockedControl(Control* control);
			// Sets the position, size, and anchors for all docked controls after each update.
			void _applyDockStyleForAllControls();
			// Marks a control for removal, which will cause it to be freed after the next update.
			void _pendControlForRemoval(collection_type::iterator control_it);
			// Frees all controls currently pending removal.
			void _eraseAllControlsPendingRemoval();

			// Sets the position, size, and anchors of the control, and adjusts the remaining dockable area accordingly.
			void _applyDockStyleToControl(Control* control);
			// Returns the mouse position with the origin at the top-left corner of the control.
			PointF _getMousePositionRelativeToControl(Control* control) const;

			GuiManager* _gui_manager;
			Control* _held_control;
			Control* _hovered_control;
			PointF _last_mouse_position;
			PointF _control_offset;
			collection_type _controls;
			std::vector<Control*> _docked_controls;
			std::vector<collection_type::iterator> _pending_removal;
			bool _resort_needed;
			bool _keyboard_events_enabled;
			bool _mouse_events_enabled;
			RectangleF _dockable_region;
			RectangleF _temp_dockable_region;

		};

	}

}