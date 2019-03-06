#pragma once
#include "hvn3/gui/WidgetBase.h"
#include "hvn3/gui/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class ContextMenuItem :
			public WidgetBase {

		public:
			ContextMenuItem(const String& text);

			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override;
			void OnMouseHover(WidgetMouseHoverEventArgs& e) override;
			void OnMouseLeave(WidgetMouseLeaveEventArgs& e) override;
			void OnUpdate(WidgetUpdateEventArgs& e) override;

			void SetVisible(bool value) override;

			bool Checked() const;
			void SetChecked(bool value);

		private:
			float _hover_timer;

			bool _mouseOnItemOrContextMenu(IWidget* widget);

		};

		class ContextMenuItemSeparator :
			public WidgetBase {

		public:
			ContextMenuItemSeparator();

		};

		class ContextMenu : public WidgetBase {

		public:
			ContextMenu();

			void AddItem(std::unique_ptr<IWidget>& item);
			void AddItem(ContextMenuItem* item);
			ContextMenuItem* AddItem(const String& text);
			ContextMenuItem* AddItem(const String& text, bool checked);
			void InsertItem(ContextMenuItem* item, int index);
			IWidget* ItemAt(int index);
			void AddSeparator();

			WidgetManager& GetChildren() override;
			bool HasChildren() override;
			size_t Count() const;

			void OnUpdate(WidgetUpdateEventArgs& e) override;
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override;

		private:
			WidgetManager _child_manager;
			bool _resize_pending;

			void _resizeItems();

		};

	}
}