#pragma once
#include "hvn3/gui2/ScrollableWidgetBase.h"
#include "hvn3/gui2/ContainerWidgetBase.h"

namespace hvn3 {
	namespace Gui {


		class ListBoxItem :
			public WidgetBase {

		public:
			ListBoxItem() {

				SetIdentifier("listboxitem");
				SetHeight(15.0f);

			}
			ListBoxItem(const String& text) :
				ListBoxItem() {

				SetText(text);

			}

			void OnFocus(WidgetFocusEventArgs& e) {

				WidgetBase::OnFocus(e);

				SetState(WidgetState::Selected, true);

				if (GetParent() != nullptr && GetParent()->HasChildren())
					for (auto i = GetParent()->GetChildren().begin(); i != GetParent()->GetChildren().begin(); ++i)
						if (i->widget.get() != this)
							i->widget->SetState(WidgetState::Selected, false);

			}

		};

		class ListBox : public ScrollableWidgetBase, public ContainerWidgetBase {

		public:
			ListBox() :
				ScrollableWidgetBase(Size()) {

				SetIdentifier("listbox");

			}

			ListBoxItem* AddItem(ListBoxItem* item) {

				GetChildren().Add(item);

				return item;

			}
			ListBoxItem* AddItem(const String& text) {

				ListBoxItem* item = new ListBoxItem(text);
				AddItem(item);

				return item;

			}

			int SelectedIndex() {

				int index = 0;

				for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i, ++index)
					if (HasFlag(i->widget->State(), WidgetState::Selected))
						return index;

				return -1;

			}

			void OnUpdate(WidgetUpdateEventArgs& e) {

				float yoff = 0.0f;

				for (auto i = GetChildren().GetWidgets().begin(); i != GetChildren().GetWidgets().end(); ++i) {

					i->widget->SetWidth(Width());
					i->widget->SetY(yoff);

					yoff += i->widget->Height();

				}

				GetChildren().SetDockableRegion(RectangleF(Size()));

			}

		};

	}
}