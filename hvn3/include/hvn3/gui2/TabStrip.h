#pragma once
#include "hvn3/gui2/ContainerWidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class TabStrip;

		class TabStripItem : public WidgetBase {
			friend class TabStrip;

		public:
			TabStripItem(const String& text);

			void OnMousePressed(WidgetMousePressedEventArgs& e) override;

		private:
			TabStrip* _owner;

		};

		class TabStrip : public ContainerWidgetBase {
			friend class TabStripItem;

		public:
			TabStrip();

			TabStripItem* AddItem(TabStripItem* item);
			TabStripItem* AddItem(const String& text);

			const TabStripItem* SelectedTab() const;
			TabStripItem* GetSelectedTab();

			virtual void OnSelectedItemChanged(WidgetSelectedTabChangedEventArgs& e);

			void OnUpdate(WidgetUpdateEventArgs& e) override;

		protected:
			void SetSelectedTab(IWidget* sender, TabStripItem* tab);

		private:
			TabStripItem* _selected_tab;

		};

	}
}