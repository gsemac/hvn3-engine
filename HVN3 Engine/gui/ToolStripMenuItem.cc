#include "gui/ToolStripMenuItem.h"
#include "gui/GuiManager.h"
#include "gui/ToolStripDropDown.h"
#include "io/Mouse.h"
#include "Random.h"
#define DEFAULT_ICON_MAX_WIDTH 25.0f
#define DEFAULT_SUBMENU_HOVER_DELAY 1.5f

namespace hvn3 {
	namespace Gui {

		ToolStripMenuItem::ToolStripMenuItem() :
			TextableControl("ToolStripMenuItem"),
			Control(Point2F(0, 0), SizeF(100, 20)) {

			_sub_menu_ptr = nullptr;
			_sub_menu_is_managed = false;
			_hover_time = 0.0f;

		}
		ToolStripMenuItem::~ToolStripMenuItem() {

			// If the sub menu was created, remove it from the manager, or delete if it isn't managed.
			if (_sub_menu_ptr)
				_DestroySubMenu();

			_sub_menu_ptr = nullptr;

		}

		bool ToolStripMenuItem::HasActiveSubMenu() const {

			// If there is no sub menu to begin with, it couldn't possibly be active.
			if (!_sub_menu_ptr)
				return false;

			// If the mouse is currently on the sub menu, consider it active.
			if (_sub_menu_ptr->Enabled() && Mouse::InRegion(_sub_menu_ptr->Bounds()))
				return true;

			// Otherwise, consider the sub menu active if it itself has an active sub menu.
			return _sub_menu_ptr->HasActiveSubMenu();

		}
		Handle<ToolStripDropDown> ToolStripMenuItem::SubMenu() {

			return _sub_menu_ptr;

		}
		void ToolStripMenuItem::AddItem(ToolStripMenuItem* item) {



		}

		void ToolStripMenuItem::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());

			if (Font())
				e.Graphics().DrawText(DEFAULT_ICON_MAX_WIDTH + 4.0f, Round(Height() / 2 - Font()->Height() / 2), Text(), Font(), ForeColor());

			// Draw icon separator.
			e.Graphics().DrawLine(DEFAULT_ICON_MAX_WIDTH, 0, DEFAULT_ICON_MAX_WIDTH, Height(), BackColor().Lighter(), 1);

			// Draw arrow indicating sub-items.
			if (_sub_menu_ptr) {
				auto icon = Manager()->StyleManager()->GetImageResource(BitmapResourceId::ArrowR);
				if (icon)
					e.Graphics().DrawBitmap(Width() - 10, Round(Height() / 2 - icon->Height() / 2), icon);
			}

		}
		void ToolStripMenuItem::OnUpdate(UpdateEventArgs& e) {

			// #todo The menu shouldn't start fading just because we mouse off of it. It should only fade if we mouse over a different menu item.

			bool show_sub_menu = (_sub_menu_ptr && (IsActiveControl() || Mouse::InRegion(_sub_menu_ptr->Bounds()) || _sub_menu_ptr->HasActiveSubMenu()));

			if (_sub_menu_ptr && !show_sub_menu && _sub_menu_ptr->Enabled()) {

				_DecrementSubMenuDestroyTimerValue(0.1f);
				if (_SubMenuDestroyTimerValue() <= 0.0f && !_sub_menu_ptr->HasVisibleSubMenu())
					_HideSubMenu();

			}
			else
				_SetSubMenuDestroyTimerValue(DEFAULT_SUBMENU_HOVER_DELAY);

		}
		void ToolStripMenuItem::OnMouseEnter() {

			_ResetSubMenuDestroyTimer();

			SetBackColor(BackColor().Lighter());

			Invalidate();

		}
		void ToolStripMenuItem::OnMouseLeave() {

			_hover_time = 0.0f;

			SetBackColor(BackColor().Darker());

			Invalidate();

		}
		void ToolStripMenuItem::OnMouseHover() {

			if (_hover_time >= DEFAULT_SUBMENU_HOVER_DELAY) {

				_ShowSubMenu();
				_hover_time = DEFAULT_SUBMENU_HOVER_DELAY;

			}
			else
				_hover_time += 0.1f;

		}
		void ToolStripMenuItem::OnLostFocus() {

		}

		// Private methods

		bool ToolStripMenuItem::_CreateSubMenu() {

			// Return false if the sub menu has already been created.
			if (_sub_menu_ptr != nullptr)
				return false;

			// Create the sub menu.
			_sub_menu_ptr = new ToolStripDropDown(Point2F(), Width());
			for (int i = 0; i < Random::Integer(2, 6); ++i)
				_sub_menu_ptr->AddItem(new ToolStripMenuItem);

		}
		void ToolStripMenuItem::_DestroySubMenu() {

			// If there is no sub menu, do nothing.
			if (!_sub_menu_ptr)
				return;

			// Attempt to remove the sub menu from its manager. If we can't (it doesn't exist), delete it ourselves.
			if (!_sub_menu_is_managed)
				delete _sub_menu_ptr;

			_sub_menu_ptr = nullptr;

		}
		void ToolStripMenuItem::_ShowSubMenu() {

			_CreateSubMenu();

			// Do nothing if a sub menu has not been created.
			if (!_sub_menu_ptr)
				return;

			// Set the position of the sub menu.
			Point2F fp = FixedPosition();
			_sub_menu_ptr->SetPosition(fp.X() + Width(), fp.Y());

			// Make the menu visible and enable it.
			_sub_menu_ptr->SetVisible(true);
			_sub_menu_ptr->SetEnabled(true);

			// Add the control to the parent control's manager.
			if (!_sub_menu_is_managed) {
				Parent()->Manager()->ControlManager()->AddControl(Control::Create(_sub_menu_ptr));
				_sub_menu_is_managed = true;
			}

		}
		void ToolStripMenuItem::_HideSubMenu() {

			// If there is no sub menu, do nothing.
			if (!_sub_menu_ptr)
				return;

			// Disable and hide the sub menu.
			_sub_menu_ptr->SetVisible(false);
			_sub_menu_ptr->SetEnabled(false);

		}
		float ToolStripMenuItem::_SubMenuDestroyTimerValue() const {

			return _sub_menu_destroy_timer;

		}
		void ToolStripMenuItem::_SetSubMenuDestroyTimerValue(float value) {

			_sub_menu_destroy_timer = value;

			if (_sub_menu_ptr)
				_sub_menu_ptr->SetOpacity(_SubMenuDestroyTimerValue() / DEFAULT_SUBMENU_HOVER_DELAY);

		}
		void ToolStripMenuItem::_DecrementSubMenuDestroyTimerValue(float amount) {

			_SetSubMenuDestroyTimerValue(Max(_SubMenuDestroyTimerValue() - amount, 0.0f));

		}
		void ToolStripMenuItem::_ResetSubMenuDestroyTimer() {

			_sub_menu_destroy_timer = 0.0f;

			if (_sub_menu_ptr)
				_sub_menu_ptr->SetOpacity(1.0f);

		}

	}
}