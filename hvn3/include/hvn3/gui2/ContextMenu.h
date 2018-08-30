#pragma once
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class ContextMenuItem :
			public WidgetBase {

		public:
			ContextMenuItem(const String& text) {

				SetId("contextmenuitem");
				SetText(text);

			}

			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override {

				WidgetBase::OnMouseReleased(e);

				// The context menu that this item belongs to should no longer be visible after the item is clicked.
				if (GetParent() != nullptr)
					GetParent()->SetVisible(false);

			}

			bool Checked() const {
				return HasFlag(State(), WidgetState::Checked);
			}
			void SetChecked(bool value) {
				SetState(WidgetState::Checked, value);
			}
			
		};

		class ContextMenuItemSeparator :
			public WidgetBase {

		public:
			ContextMenuItemSeparator() {
				SetId("contextmenuitemseparator");
			}

		};

		class ContextMenu : public WidgetBase {

		public:
			ContextMenu() :
				_child_manager(this) {

				SetId("contextmenu");

			}

			void AddItem(std::unique_ptr<IWidget>& item) {

				item->SetParent(this);

				_child_manager.Add(item);

			}
			void AddItem(ContextMenuItem* item) {
				AddItem(std::unique_ptr<IWidget>(item));
			}
			ContextMenuItem* AddItem(const String& text) {

				ContextMenuItem* item = new ContextMenuItem(text);
				AddItem(item);

				return item;

			}
			ContextMenuItem* AddItem(const String& text, bool checked) {

				ContextMenuItem* item = AddItem(text);
				item->SetChecked(checked);

				return item;

			}
			void AddSeparator() {
				AddItem(std::unique_ptr<IWidget>(new ContextMenuItemSeparator));
			}

			WidgetManager& GetChildren() override {
				return _child_manager;
			}
			bool HasChildren() override {
				return _child_manager.Count() > 0;
			}

			void OnUpdate(WidgetUpdateEventArgs& e) override {
				WidgetBase::OnUpdate(e);

				// Update the dockable region for child widgets (in case this widget's dimensions have changed).
				_child_manager.SetDockableRegion(RectangleF(0.0f, 0.0f, Width(), Height()));


			}
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override {
				WidgetBase::OnRendererChanged(e);

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

		private:
			WidgetManager _child_manager;

		};

	}
}