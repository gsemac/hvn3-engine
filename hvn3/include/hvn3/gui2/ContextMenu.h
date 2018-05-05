#pragma once
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class ContextMenuItem : public WidgetBase {

		public:
			ContextMenuItem(const String& text) {

				SetIdentifier("contextmenuitem");
				SetText(text);

			}

		};

		class ContextMenu : public WidgetBase {

		public:
			ContextMenu() :
				_child_manager(this) {

				SetIdentifier("contextmenu");

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

				for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

					SizeF text_size = GetManager()->Renderer().MeasureString(i->widget->Text());

					if (text_size.width > max_width)
						max_width = text_size.width;

					if (text_size.height > max_height)
						max_height = text_size.height;

				}

				SetSize(max_width + (padding * 2.0f), (max_height + (padding * 2.0f)) * _child_manager.Count());
				_child_manager.SetDockableRegion(RectangleF(Size()));

				// Update the dimensions of all items.

				float margin_y = 0.0f;
				float y = margin_y;

				for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

					i->widget->SetSize(max_width + (padding * 2.0f), max_height + (padding * 2.0f));
					i->widget->SetPosition(0.0f, y);

					y += margin_y + max_height + (padding * 2.0f);

				}

			}

		private:
			WidgetManager _child_manager;

		};

	}
}