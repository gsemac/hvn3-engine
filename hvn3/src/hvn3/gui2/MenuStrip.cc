#include "hvn3/gui2/MenuStrip.h"

namespace hvn3 {
	namespace Gui {

		MenuStripItem::MenuStripItem(MenuStrip* parent, const String& text) {

			SetIdentifier("menustripitem");
			SetText(text);

			_parent = parent;
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

			if (!_context_menu_managed && _context_menu != nullptr && _parent != nullptr && _parent->GetManager() != nullptr) {
				_parent->GetManager()->Add(_context_menu);
				_context_menu_managed = true;
			}

			if (_context_menu != nullptr) {

				_context_menu->SetPosition(Position().x, Position().y + Height());
				_context_menu->SetVisible(true);

				if (_parent->GetManager() != nullptr)
					_parent->GetManager()->BringToFront(_context_menu);

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

			if (_parent == nullptr || _parent->GetManager() == nullptr)
				return;

			_parent->GetManager()->Remove(_context_menu);

			_context_menu = nullptr;

		}
		bool MenuStripItem::ContextMenuVisible() const {

			if (_context_menu == nullptr)
				return false;

			return _context_menu->Visible();

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

			SetIdentifier("menustrip");
			SetDockStyle(DockStyle::Top);
			SetHeight(height);

			_active_item = nullptr;

		}
		void MenuStrip::AddItem(std::unique_ptr<IWidget>& item) {
			_child_manager.Add(item);
		}
		void MenuStrip::AddItem(MenuStripItem* item) {
			AddItem(std::unique_ptr<IWidget>(item));
		}
		MenuStripItem* MenuStrip::AddItem(const String& text) {

			MenuStripItem* item = new MenuStripItem(this, text);
			AddItem(item);

			return item;

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

			if (GetManager() == nullptr)
				return;

			// Resize all child widgets according to the new renderer.

			float margin_x = 2.0f;
			float margin_y = 1.0f;
			float padding = 8.0f;
			float min_width = Height() - (padding * 2.0f);

			float x = margin_x;
			float y = margin_y;

			for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

				SizeF text_size = GetManager()->Renderer().MeasureString(i->widget->Text());

				i->widget->SetPosition(x, y);
				i->widget->SetSize((std::max)(min_width, text_size.width) + (padding * 2.0f), Height());

				x += margin_x + i->widget->Width();

			}

		}

	}
}