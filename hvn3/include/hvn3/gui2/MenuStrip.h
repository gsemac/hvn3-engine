#pragma once
#include "hvn3/gui2/ContextMenu.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class MenuStrip;

		class MenuStripItem : public WidgetBase {
			friend class MenuStrip;

		public:
			MenuStripItem(MenuStrip* parent, const String& text);

			void SetContextMenu(ContextMenu* context_menu);

			void OnMouseUp(WidgetMouseEventArgs& e) override;

		protected:
			void ShowContextMenu();
			void HideContextMenu();

		private:
			MenuStrip* _parent;
			ContextMenu* _context_menu;
			bool _context_menu_visible;

		};

		class MenuStrip : public WidgetBase {
			friend class MenuStripItem;

		public:
			MenuStrip();
			MenuStrip(float height);

			void AddItem(std::unique_ptr<IWidget>& item);
			void AddItem(MenuStripItem* item);
			MenuStripItem* AddItem(const String& text);

			WidgetManager& GetChildren() override;
			bool HasChildren() override;

			void OnUpdate(WidgetUpdateEventArgs& e) override;
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override;

		private:
			WidgetManager _child_manager;
			MenuStripItem* _active_item;

			void _showContextMenu(ContextMenu* cm);

		};

	}
}