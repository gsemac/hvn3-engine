#include "hvn3/gui2/MenuStrip.h"

/*

A lot of the functionality here is duplicated in WidgetBase, since it was made possible to arbitrarily add a context menu to any widget.
This could probably be cleaned up a bit to take advantage of that.

*/

namespace hvn3 {
	namespace Gui {

		MenuStripItem::MenuStripItem(const String& text) {

			SetId("menustripitem");
			SetText(text);

			_context_menu = nullptr;
			_context_menu_managed = false;

		}
		void MenuStripItem::SetContextMenu(ContextMenu* context_menu) {

			DestroyContextMenu();

			if (context_menu == nullptr)
				return;

			_context_menu = context_menu;
			_context_menu->SetVisible(false);

			_context_menu_managed = false;

		}
		ContextMenu* MenuStripItem::GetContextMenu() {

			return _context_menu;

		}
		void MenuStripItem::OnFocus(WidgetFocusEventArgs& e) {

			WidgetBase::OnFocus(e);

			ShowContextMenu();

		}
		void MenuStripItem::OnFocusLost(WidgetFocusLostEventArgs& e) {

			WidgetBase::OnFocusLost(e);

			// If the context menu or its children received focus, postpone closing the context menu until the next mouse-up.
			// This gives events like OnMouseClick a chance to be detected before the menu disappears.

			if (!_contextMenuOrItsChildrenHaveFocus())
				HideContextMenu();

		}
		void MenuStripItem::OnMousePressed(WidgetMousePressedEventArgs& e) {

			WidgetBase::OnMousePressed(e);

			ToggleContextMenuVisibility();

		}
		void MenuStripItem::OnMouseReleased(WidgetMouseReleasedEventArgs& e) {

			// If the item was clicked and the context menu hidden, remove focus from the item.
			// This is so that dragging to other items doesn't open their context menus if one isn't already open.
			// This is handled in OnMouseReleased instead of OnMousePressed so it doesn't just regain focus immediately after the click.

			if (!ContextMenuVisible() && GetManager() != nullptr)
				GetManager()->SetFocus(nullptr);

		}
		void MenuStripItem::OnMouseEnter(WidgetMouseEnterEventArgs& e) {

			WidgetBase::OnMouseEnter(e);

			// If any sibling items are focused, make this the focused item instead.
			// This allows moving the cursor across the menu strip to open different context menus.

			if (GetParent() == nullptr || !GetParent()->HasChildren())
				return;

			for (auto i = GetParent()->GetChildren().begin(); i != GetParent()->GetChildren().end(); ++i)
				if (HasFlag(i->widget->State(), WidgetState::Focus)) {

					GetParent()->GetChildren().SetFocus(this);

					break;

				}

		}

		void MenuStripItem::ShowContextMenu() {

			WidgetManager* manager = _getTopManager();

			if (!_context_menu_managed && _context_menu != nullptr && manager != nullptr) {
				manager->Add(_context_menu);
				_context_menu_managed = true;
			}

			if (_context_menu != nullptr) {

				PointF pos = Bounds().Position();

				_context_menu->SetPosition(pos.x, pos.y + Height());
				_context_menu->SetVisible(true);

				if (manager != nullptr)
					manager->BringToFront(_context_menu);

			}

		}
		void MenuStripItem::HideContextMenu() {

			if (_context_menu == nullptr)
				return;

			_context_menu->SetVisible(false);

		}
		void MenuStripItem::ToggleContextMenuVisibility() {

			if (ContextMenuVisible())
				HideContextMenu();
			else
				ShowContextMenu();

		}
		void MenuStripItem::DestroyContextMenu() {

			if (_context_menu == nullptr)
				return;

			// If the context menu isn't currently managed, just delete it.
			if (!_context_menu_managed) {
				delete _context_menu;
				_context_menu = nullptr;
			}

			WidgetManager* manager = _getTopManager();

			if (manager == nullptr)
				return;

			manager->Remove(_context_menu);

			_context_menu = nullptr;

		}
		bool MenuStripItem::ContextMenuVisible() const {

			if (_context_menu == nullptr)
				return false;

			return _context_menu->Visible();

		}

		WidgetManager* MenuStripItem::_getTopManager() const {

			const IWidget* ptr = this;

			while (ptr->GetParent() != nullptr)
				ptr = ptr->GetParent();

			return ptr->GetManager();

		}
		bool MenuStripItem::_contextMenuOrItsChildrenHaveFocus() const {

			if (!ContextMenuVisible())
				return false;

			if (HasFlag(_context_menu->State(), WidgetState::Focus))
				return true;

			if (_context_menu->HasChildren())
				for (auto i = _context_menu->GetChildren().begin(); i != _context_menu->GetChildren().end(); ++i)
					if (HasFlag(i->widget->State(), WidgetState::Focus))
						return true;

			return false;

		}


		MenuStrip::MenuStrip() :
			MenuStrip(25.0f) {
		}
		MenuStrip::MenuStrip(float height) :
			_child_manager(this) {

			SetId("menustrip");
			SetDockStyle(DockStyle::Top);
			SetHeight(height);

			_active_item = nullptr;

		}
		void MenuStrip::AddItem(std::unique_ptr<IWidget>& item) {

			item->SetParent(this);

			_child_manager.Add(item);

		}
		MenuStripItem* MenuStrip::AddItem(const String& text) {

			MenuStripItem* item = new MenuStripItem(text);
			AddItem(item);

			return item;

		}
		void MenuStrip::AddItem(MenuStripItem* item) {
			AddItem(std::unique_ptr<IWidget>(item));
		}
		void MenuStrip::AddItem(Label* item) {
			AddItem(std::unique_ptr<IWidget>(item));
		}
		WidgetManager& MenuStrip::GetChildren() {
			return _child_manager;
		}
		bool MenuStrip::HasChildren() {
			return _child_manager.Count() > 0;
		}
		void MenuStrip::OnUpdate(WidgetUpdateEventArgs& e) {
			WidgetBase::OnUpdate(e);

			// Update the dockable region for child widgets (in case this widget's dimensions have changed).
			_child_manager.SetDockableRegion(RectangleF(0.0f, 0.0f, Width(), Height()));

		}
		void MenuStrip::OnRendererChanged(WidgetRendererChangedEventArgs& e) {
			WidgetBase::OnRendererChanged(e);

			if (GetManager() == nullptr || !static_cast<const WidgetManager*>(GetManager())->Renderer())
				return;

			// Resize all child widgets according to the new renderer.

			float margin_x = 2.0f;
			float margin_y = 1.0f;
			float padding = 8.0f;
			float min_width = Height() - (padding * 2.0f);

			float x = margin_x;
			float y = margin_y;

			for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

				SizeF text_size = GetManager()->Renderer()->MeasureString(i->widget->Text());

				i->widget->SetPosition(x, y);
				i->widget->SetSize((std::max)(min_width, text_size.width) + (padding * 2.0f), Height());

				x += margin_x + i->widget->Width();

			}

		}

	}
}