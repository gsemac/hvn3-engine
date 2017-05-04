#pragma once
#include "IUpdatable.h"
#include "IDrawable.h"
#include "Control.h"
#include "GuiStyleManager.h"
#include <list>
#include <memory>

namespace Gui {

	class GuiManager : public IUpdatable, public IDrawable {

		typedef std::unique_ptr<Control> control_type;

	public:
		GuiManager();
		~GuiManager();

		// Adds a new control to the manager.
		void AddControl(control_type& control);
		// Removes a control from the manager. Does nothing if the control does not exist.
		void RemoveControl(Control* control);
		Control* ActiveControl();
		// Returns the control at the given index.
		Control* ControlAt(size_t index);
		// Returns the number of controls belonging to the manager.
		size_t ControlCount() const;
		void Clear();
		void BringToFront(Control* control);
		void SendToBack(Control* control);

		void SetkeyboardEventsEnabled(bool value);
		void SetMouseEventsEnabled(bool value);

		float Scale() const;
		void SetScale(float scale);
		
		GuiStyleManager* StyleManager();

		void Update(UpdateEventArgs& e) override;
		void Draw(DrawEventArgs& e) override;

	protected:
		std::list<control_type>::iterator FindControlByAddress(Control* ptr);
		// Resorts the Controls by their Z coordinate (largest first).
		void Sort();

	private:
		Control* _held_control;
		Control* _hovered_control;
		Point _last_mouse_position;
		std::list<control_type> _controls;
		bool _resort_needed;
		bool _keyboard_events_enabled;
		bool _mouse_events_enabled;
		float _gui_scale;

		GuiStyleManager _style_manager;
		
	};

}