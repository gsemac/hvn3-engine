#include "hvn3/gui2/TabStrip.h"

namespace hvn3 {
	namespace Gui {

		TabStripItem::TabStripItem(const String& text) {
			
			_owner = nullptr;
			
			SetIdentifier("tabstripitem");
			SetText(text);

		}
		void TabStripItem::OnMousePressed(WidgetMousePressedEventArgs& e) {

			WidgetBase::OnMousePressed(e);

			if (_owner != nullptr) {
				_owner->_selected_tab = this;
				_owner->OnSelectedItemChanged(WidgetSelectedTabChangedEventArgs(_owner, this));
			}

		}


		TabStrip::TabStrip() {

			_selected_tab = nullptr;

			SetIdentifier("tabstrip");
			SetHeight(25.0f);

		}
		TabStripItem* TabStrip::AddItem(TabStripItem* item) {

			item->_owner = this;
			GetChildren().Add(item);

			return item;

		}
		TabStripItem* TabStrip::AddItem(const String& text) {

			TabStripItem* item = new TabStripItem(text);
			AddItem(item);

			return item;

		}
		const TabStripItem* TabStrip::SelectedTab() const {
			return _selected_tab;
		}
		TabStripItem* TabStrip::GetSelectedTab() {
			return _selected_tab;
		}
		void TabStrip::OnSelectedItemChanged(WidgetSelectedTabChangedEventArgs& e) {}
		void TabStrip::OnUpdate(WidgetUpdateEventArgs& e)  {
			WidgetBase::OnUpdate(e);

			// Evenly space out all tabs over the width of the tab strip.
			float width = Width() / GetChildren().Count();
			float x_offset = 0.0f;

			for (auto i = GetChildren().GetWidgets().begin(); i != GetChildren().GetWidgets().end(); ++i) {
				i->widget->SetWidth(width);
				i->widget->SetHeight(Height());
				i->widget->SetX(x_offset);
				x_offset += width;
			}

			GetChildren().SetDockableRegion(RectangleF(Size()));

		}

	void TabStrip::SetSelectedTab(IWidget* sender, TabStripItem* tab) {

		_selected_tab = tab;

		OnSelectedItemChanged(WidgetSelectedTabChangedEventArgs(sender, tab));

	}

	}
}