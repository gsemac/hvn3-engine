#include "hvn3/gui2/ContextMenu.h"

namespace hvn3 {
	namespace Gui {

		ContextMenuItem::ContextMenuItem(const String& text) {

			SetId("contextmenuitem");
			SetText(text);

			_hover_timer = 0.0f;

		}
		void ContextMenuItem::OnMouseReleased(WidgetMouseReleasedEventArgs& e) {

			WidgetBase::OnMouseReleased(e);

			// The context menu that this item belongs to should no longer be visible after the item is clicked.
			if (GetParent() != nullptr)
				GetParent()->SetVisible(false);

		}
		void ContextMenuItem::OnMouseHover(WidgetMouseHoverEventArgs& e) {

			WidgetBase::OnMouseHover(e);

		}
		void ContextMenuItem::OnMouseLeave(WidgetMouseLeaveEventArgs& e) {

			WidgetBase::OnMouseLeave(e);

		}
		void ContextMenuItem::OnUpdate(WidgetUpdateEventArgs& e) {

			WidgetBase::OnUpdate(e);

			// #todo Could perform mouse check in OnMouseLeave instead to improve efficiency.

			if (_mouseOnItemOrContextMenu(this)) {

				if (_hover_timer < 1.0f)
					_hover_timer += 0.1f;
				else {

					if (GetContextMenu() != nullptr && !GetContextMenu()->Visible()) {


						PointF pos(FixedPosition().x + Width(), FixedPosition().y);
						ShowContextMenu(pos);

						// If the context menu would be over the edge of screen, show it on the other side of the item (i.e., left instead of right).

						WidgetManager* manager = GetContextMenu()->GetManager();

						if (manager != nullptr) {

							RectangleF bounds(pos.x, pos.y, GetContextMenu()->Width(), GetContextMenu()->Height());
							RectangleF region = manager->DockableRegion();

							float x_over = bounds.Right() - region.Right();
							float y_over = bounds.Bottom() - region.Bottom();

							if (x_over > 0.0f || y_over > 0.0f)
								ShowContextMenu(PointF(FixedPosition().x - GetContextMenu()->Width(), FixedPosition().y));

						}

					}

				}

			}
			else {

				if (_hover_timer > 0.0f)
					_hover_timer -= 0.1f;
				else {

					if (GetContextMenu() != nullptr && GetContextMenu()->Visible())
						GetContextMenu()->SetVisible(false);

				}

			}

		}
		void ContextMenuItem::SetVisible(bool value) {

			WidgetBase::SetVisible(value);

			if (!Visible() && GetContextMenu() != nullptr)
				GetContextMenu()->SetVisible(false);

		}
		bool ContextMenuItem::Checked() const {
			return HasFlag(State(), WidgetState::Checked);
		}
		void ContextMenuItem::SetChecked(bool value) {
			SetState(WidgetState::Checked, value);
		}
		bool ContextMenuItem::_mouseOnItemOrContextMenu(IWidget* widget) {

			if (HasFlag(widget->State(), WidgetState::Hover))
				return true;

			if (widget->GetContextMenu() == nullptr || !widget->GetContextMenu()->Visible())
				return false;

			if (HasFlag(widget->GetContextMenu()->State(), WidgetState::Hover))
				return true;

			if (widget->GetContextMenu()->HasChildren()) {

				for (auto i = widget->GetContextMenu()->GetChildren().begin(); i != widget->GetContextMenu()->GetChildren().end(); ++i) {

					if (_mouseOnItemOrContextMenu(i->widget.get()))
						return true;

				}

			}

			return false;

		}


		ContextMenuItemSeparator::ContextMenuItemSeparator() {
			SetId("contextmenuitemseparator");
		}



		ContextMenu::ContextMenu() :
			_child_manager(this) {

			SetId("contextmenu");
			_resize_pending = false;

		}
		void ContextMenu::AddItem(std::unique_ptr<IWidget>& item) {

			item->SetParent(this);

			_child_manager.Add(item);

			_resizeItems();

		}
		void ContextMenu::AddItem(ContextMenuItem* item) {
			AddItem(std::unique_ptr<IWidget>(item));
		}
		ContextMenuItem* ContextMenu::AddItem(const String& text) {

			ContextMenuItem* item = new ContextMenuItem(text);
			AddItem(item);

			return item;

		}
		ContextMenuItem* ContextMenu::AddItem(const String& text, bool checked) {

			ContextMenuItem* item = AddItem(text);
			item->SetChecked(checked);

			return item;

		}
		void ContextMenu::InsertItem(ContextMenuItem* item, int index) {

			assert(index >= 0);

			AddItem(item);

			// #todo Implement this properly, because this doesn't actually work.

			item->BringToFront();

			size_t c_index = _child_manager.Count();
			auto range_begin = _child_manager.rbegin();
			auto range_end = _child_manager.rend();

			for (auto i = range_begin; i != range_end && c_index > static_cast<size_t>(index); ++i, --c_index);

			for (auto i = range_begin; i != range_end; ++i)
				i->widget->BringToFront();

			_resize_pending = true;

		}
		IWidget* ContextMenu::ItemAt(int index) {

			if (index < 0 || static_cast<size_t>(index) > Count())
				return nullptr;

			int c_index = 0;

			for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i, ++c_index)
				if (c_index == index)
					return i->widget.get();

			return nullptr;

		}
		void ContextMenu::AddSeparator() {
			AddItem(std::unique_ptr<IWidget>(new ContextMenuItemSeparator));
		}
		WidgetManager& ContextMenu::GetChildren() {
			return _child_manager;
		}
		bool ContextMenu::HasChildren() {
			return _child_manager.Count() > 0;
		}
		size_t ContextMenu::Count() const {

			return _child_manager.Count();

		}
		void ContextMenu::OnUpdate(WidgetUpdateEventArgs& e) {

			WidgetBase::OnUpdate(e);

			// Update the dockable region for child widgets (in case this widget's dimensions have changed).
			_child_manager.SetDockableRegion(RectangleF(0.0f, 0.0f, Width(), Height()));

			if (_resize_pending) {

				_resizeItems();
				_resize_pending = false;

			}

		}
		void ContextMenu::OnRendererChanged(WidgetRendererChangedEventArgs& e) {

			WidgetBase::OnRendererChanged(e);

			_resizeItems();

		}

		void ContextMenu::_resizeItems() {

			if (GetManager() == nullptr)
				return;

			// We need to find the widest child widget to determine the width of the menu.
			// The maximum text height will determine the height for all items.

			float max_width = 0.0f;
			float max_height = 0.0f;
			float padding = 4.0f;
			float icon_padding = 30.0f + 4.0f; // offer 4 pixels text padding after the icon area
			float separator_height = 5.0f;

			int item_count = 0;
			int separator_count = 0;


			for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

				if (i->widget->Id() == "contextmenuitemseparator") {
					++separator_count;
					continue;
				}

				++item_count;

				SizeF text_size = GetManager()->Renderer()->MeasureString(i->widget->Text());

				if (text_size.width > max_width)
					max_width = text_size.width;

				if (text_size.height > max_height)
					max_height = text_size.height;

			}

			float item_width = max_width + (padding * 2.0f) + icon_padding;
			float item_height = max_height + (padding * 2.0f);

			item_width = Math::Max(item_width, 142.0f);
			item_height = Math::Max(item_height, 25.0f);

			SetSize(item_width, item_height * item_count + separator_height * separator_count);
			_child_manager.SetDockableRegion(RectangleF(Size()));

			// Update the dimensions of all items.

			float margin_y = 0.0f;
			float y = margin_y;

			for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

				i->widget->SetWidth(item_width);
				i->widget->SetPosition(0.0f, y);

				if (i->widget->Id() == "contextmenuitemseparator")
					i->widget->SetHeight(separator_height);
				else
					i->widget->SetHeight(item_height);

				y += margin_y + i->widget->Height();

			}

		}

	}
}