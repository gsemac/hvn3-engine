//#pragma once
//#include "hvn3/gui/ContextMenu.h"
//#include "hvn3/gui/IWidgetRenderer.h"
//#include "hvn3/gui/Label.h"
//#include "hvn3/gui/WidgetBase.h"
//
//namespace hvn3 {
//	namespace Gui {
//
//		class MenuStrip;
//
//		class MenuStripItem : public WidgetBase {
//			friend class MenuStrip;
//
//		public:
//			MenuStripItem(const String& text);
//
//			void SetContextMenu(ContextMenu* context_menu) override;
//			ContextMenu* GetContextMenu() override;
//
//			void OnFocus(WidgetFocusEventArgs& e) override;
//			void OnFocusLost(WidgetFocusLostEventArgs& e) override;
//			void OnMousePressed(WidgetMousePressedEventArgs& e) override;
//			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override;
//			void OnMouseEnter(WidgetMouseEnterEventArgs& e) override;
//
//		protected:
//			void ShowContextMenu();
//			void HideContextMenu();
//			void ToggleContextMenuVisibility();
//			void DestroyContextMenu();
//			bool ContextMenuVisible() const;
//
//		private: 
//			ContextMenu* _context_menu;
//			bool _context_menu_managed;
//
//			WidgetManager* _getTopManager() const;
//			bool _contextMenuOrItsChildrenHaveFocus() const;
//
//		};
//
//		class MenuStrip : public WidgetBase {
//			friend class MenuStripItem;
//
//		public:
//			MenuStrip();
//			MenuStrip(float height);
//
//			void AddItem(std::unique_ptr<IWidget>& item);
//			MenuStripItem* AddItem(const String& text);
//			void AddItem(MenuStripItem* item);
//			void AddItem(Label* item);
//
//			WidgetManager& GetChildren() override;
//			bool HasChildren() override;
//
//			void OnUpdate(WidgetUpdateEventArgs& e) override;
//			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override;
//
//		private:
//			WidgetManager _child_manager;
//			MenuStripItem* _active_item;
//
//		};
//
//	}
//}