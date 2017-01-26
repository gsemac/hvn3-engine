//#ifndef __GUIMANAGER_H
//#define __GUIMANAGER_H
//#include <allegro5\allegro.h>
//#include <list>
//#include <memory>
//#include "Object.h"
//#include "Control.h"
//
//namespace Gui {
//	
//	class GuiManager : public Object {
//
//	public:
//		GuiManager();
//
//		void AddControl(Control* control);
//		void RemoveControl(Control*& control);
//		Control* ActiveControl();
//		void BringToFront(Control* control);
//		void SendToBack(Control* control);
//
//		void SetkeyboardEventsEnabled(bool value);
//		void SetMouseEventsEnabled(bool value);
//		
//		std::list<Control*>& Controls();
//
//		float Scale();
//		void SetScale(float scale);
//
//		void Update(float dt = 1.0f) override;
//		void Draw() override;
//
//	private:
//		Control* __held_control;
//		Control* __hovered_control;
//		Point __mouse_last_pos;
//		std::list<Control*> __controls;
//		bool __resort_needed;
//		bool __keyboard_events_enabled, __mouse_events_enabled;
//		float __gui_scale;
//
//		// Resorts the Controls by their Z coordinate (largest first).
//		void Sort();
//
//	};
//
//}
//
//#endif