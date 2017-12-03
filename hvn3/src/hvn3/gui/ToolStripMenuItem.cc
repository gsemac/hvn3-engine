#include "hvn3/gui/ToolStripMenuItem.h"
#include "hvn3/gui/GuiManager.h"
#include "hvn3/gui/ToolStripDropDown.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/utility/Random.h"
#define DEFAULT_ICON_MAX_WIDTH 25.0f
#define DEFAULT_SUBMENU_HOVER_DELAY 1.5f

namespace hvn3 {
	namespace Gui {

		ToolStripMenuItem::ToolStripMenuItem() :
			TextableControl("ToolStripMenuItem"),
			Control(PointF(0, 0), SizeF(100, 20)) {

			_sub_menu_ptr = nullptr;
			_sub_menu_is_managed = false;
			_hover_time = 0.0f;

		}
		ToolStripMenuItem::~ToolStripMenuItem() {

			// If the sub menu was created, remove it from the manager, or delete if it isn't managed.
			if (_sub_menu_ptr)
				_destroySubMenu();

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
				e.Graphics().DrawText(DEFAULT_ICON_MAX_WIDTH + 4.0f, Math::Round(Height() / 2 - Font()->Height() / 2), Text(), Font(), ForeColor());

			// Draw icon separator.
			e.Graphics().DrawLine(DEFAULT_ICON_MAX_WIDTH, 0, DEFAULT_ICON_MAX_WIDTH, Height(), BackColor().Lighter(), 1);

			// Draw arrow indicating sub-items.
			if (_sub_menu_ptr) {
				auto icon = Manager()->StyleManager()->GetImageResource(BitmapResourceId::ArrowR);
				if (icon)
					e.Graphics().DrawBitmap(Width() - 10, Math::Round(Height() / 2 - icon->Height() / 2), icon);
			}

		}
		void ToolStripMenuItem::OnUpdate(UpdateEventArgs& e) {

			// #todo The menu shouldn't start fading just because we mouse off of it. It should only fade if we mouse over a different menu item.

			bool show_sub_menu = (_sub_menu_ptr && (IsActiveControl() || Mouse::InRegion(_sub_menu_ptr->Bounds()) || _sub_menu_ptr->HasActiveSubMenu()));

			if (_sub_menu_ptr && !show_sub_menu && _sub_menu_ptr->Enabled()) {

				_decrementSubMenuDestroyTimerValue(0.1f);
				if (_subMenuDestroyTimerValue() <= 0.0f && !_sub_menu_ptr->HasVisibleSubMenu())
					_hideSubMenu();

			}
			else
				_setSubMenuDestroyTimerValue(DEFAULT_SUBMENU_HOVER_DELAY);

		}
		void ToolStripMenuItem::OnMouseEnter() {

			_resetSubMenuDestroyTimer();

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

				_showSubMenu();
				_hover_time = DEFAULT_SUBMENU_HOVER_DELAY;

			}
			else
				_hover_time += 0.1f;

		}
		void ToolStripMenuItem::OnLostFocus() {

		}

		// Private methods

		bool ToolStripMenuItem::_createSubMenu() {

			// Return false if the sub menu has already been created.
			if (_sub_menu_ptr != nullptr)
				return false;

			// Create the sub menu.
			_sub_menu_ptr = new ToolStripDropDown(PointF(), Width());
			for (int i = 0; i < Random::Integer(2, 6); ++i)
				_sub_menu_ptr->AddItem(new ToolStripMenuItem);

			// Return true to indicate that a new sub menu has been created.
			return true;

		}
		void ToolStripMenuItem::_destroySubMenu() {

			// If there is no sub menu, do nothing.
			if (!_sub_menu_ptr)
				return;

			// Attempt to remove the sub menu from its manager. If we can't (it doesn't exist), delete it ourselves.
			if (!_sub_menu_is_managed)
				delete _sub_menu_ptr;

			_sub_menu_ptr = nullptr;

		}
		void ToolStripMenuItem::_showSubMenu() {

			_createSubMenu();

			// Do nothing if a sub menu has not been created.
			if (!_sub_menu_ptr)
				return;

			// Set the position of the sub menu.
			PointF fp = FixedPosition();
			_sub_menu_ptr->SetPosition(fp.X() + Width(), fp.Y());

			// Make the menu visible and enable it.
			_sub_menu_ptr->SetVisible(true);
			_sub_menu_ptr->SetEnabled(true);

			// Add the control to the parent control's manager.
			if (!_sub_menu_is_managed) {
				Parent()->Manager()->ControlManager()->AddControl(ControlPtr(_sub_menu_ptr));
				_sub_menu_is_managed = true;
			}

		}
		void ToolStripMenuItem::_hideSubMenu() {

			// If there is no sub menu, do nothing.
			if (!_sub_menu_ptr)
				return;

			// Disable and hide the sub menu.
			_sub_menu_ptr->SetVisible(false);
			_sub_menu_ptr->SetEnabled(false);

		}
		float ToolStripMenuItem::_subMenuDestroyTimerValue() const {

			return _sub_menu_destroy_timer;

		}
		void ToolStripMenuItem::_setSubMenuDestroyTimerValue(float value) {

			_sub_menu_destroy_timer = value;

			if (_sub_menu_ptr)
				_sub_menu_ptr->SetOpacity(_subMenuDestroyTimerValue() / DEFAULT_SUBMENU_HOVER_DELAY);

		}
		void ToolStripMenuItem::_decrementSubMenuDestroyTimerValue(float amount) {

			_setSubMenuDestroyTimerValue(Math::Max(_subMenuDestroyTimerValue() - amount, 0.0f));

		}
		void ToolStripMenuItem::_resetSubMenuDestroyTimer() {

			_sub_menu_destroy_timer = 0.0f;

			if (_sub_menu_ptr)
				_sub_menu_ptr->SetOpacity(1.0f);

		}

	}
}