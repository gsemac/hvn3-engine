#pragma once
#include "gui/TextableControl.h"
#include "gui/Handle.h"
#include <vector>

namespace hvn3 {
	namespace Gui {

		class ToolStripDropDown;

		class ToolStripMenuItem : public TextableControl {

		public:
			ToolStripMenuItem();
			~ToolStripMenuItem();
			
			virtual bool HasActiveSubMenu() const;
			Handle<ToolStripDropDown> SubMenu();
			virtual void AddItem(ToolStripMenuItem* item);

			virtual void OnPaint(PaintEventArgs& e) override;
			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseHover() override;
			virtual void OnLostFocus() override;

		protected:
			
		private:
			bool _CreateSubMenu();
			void _DestroySubMenu();
			void _ShowSubMenu();
			void _HideSubMenu();
			float _SubMenuDestroyTimerValue() const;
			void _SetSubMenuDestroyTimerValue(float value);
			void _DecrementSubMenuDestroyTimerValue(float amount);
			void _ResetSubMenuDestroyTimer();

			ToolStripDropDown* _sub_menu_ptr;
			float _sub_menu_is_managed;
			float _hover_time;
			float _sub_menu_destroy_timer;

		};

	}
}