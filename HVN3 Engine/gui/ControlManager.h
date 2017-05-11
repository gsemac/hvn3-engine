#pragma once
#include <list>
#include <vector>
#include <memory>
#include "IUpdatable.h"
#include "IDrawable.h"
#include "Point.h"
#include "Handle.h"
#include "gui/Control.h"

namespace Gui {

	//class Control;
	class GuiManager;

	class ControlManager : public IUpdatable, public IDrawable {

	public:
		ControlManager(GuiManager* gui_manager);
		virtual ~ControlManager();

		// Adds a new control to the manager.
		virtual Handle<Control> AddControl(ControlPtr& control);
		// Removes a control from the manager. Does nothing if the control does not exist.
		virtual void RemoveControl(const Handle<Control>& control);
		Handle<Control> ActiveControl();
		const Handle<Control> ActiveControl() const;
		void ClearActiveControl();
		// Returns the control at the given index.
		Handle<Control> ControlAt(size_t index);
		// Returns the number of controls belonging to the manager.
		size_t ControlCount() const;

		void Clear();
		void BringToFront(const Handle<Control>& control);
		void SendToBack(const Handle<Control>& control);

		void SetkeyboardEventsEnabled(bool value);
		void SetMouseEventsEnabled(bool value);
		void InvalidateAll();

		virtual void Update(UpdateEventArgs& e) override;
		virtual void Draw(DrawEventArgs& e) override;

	protected:
		std::list<ControlPtr>::iterator FindControlByAddress(Control* ptr);
		// Resorts the Controls by their Z coordinate (largest first).
		void Sort();

	public:
		GuiManager* _gui_manager;
		Control* _held_control;
		Control* _hovered_control;
		Point _last_mouse_position;
		std::list<ControlPtr> _controls;
		std::vector<std::list<ControlPtr>::iterator> _pending_removal;
		bool _resort_needed;
		bool _keyboard_events_enabled;
		bool _mouse_events_enabled;

	};

}