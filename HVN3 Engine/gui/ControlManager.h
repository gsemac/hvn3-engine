#pragma once
#include <list>
#include <vector>
#include <memory>
#include "IUpdatable.h"
#include "IDrawable.h"
#include "Point.h"
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
			ControlManager(GuiManager* gui_manager);
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
			const Point& ControlOffset() const;
			void SetkeyboardEventsEnabled(bool value);
			void SetMouseEventsEnabled(bool value);
			void InvalidateAll();

			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnDraw(DrawEventArgs& e) override;

		protected:
			collection_type::iterator FindControlByAddress(Control* ptr);
			// Resorts the Controls by their Z coordinate (largest first).
			void Sort();

		private:
			GuiManager* _gui_manager;
			Control* _held_control;
			Control* _hovered_control;
			Point _last_mouse_position;
			Point _control_offset;
			collection_type _controls;
			std::vector<collection_type::iterator> _pending_removal;
			bool _resort_needed;
			bool _keyboard_events_enabled;
			bool _mouse_events_enabled;

		};

	}

}