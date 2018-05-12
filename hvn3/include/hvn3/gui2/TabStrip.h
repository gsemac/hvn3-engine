#pragma once
#include "hvn3/gui2/ContainerWidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class TabStripItem : public WidgetBase {

		public:
			TabStripItem(const String& text) {
				SetIdentifier("tabstripitem");
				SetText(text);
			}			

		};

		class TabStrip : public ContainerWidgetBase {

		public:
			TabStrip() {
				SetIdentifier("tabstrip");
				SetHeight(25.0f);
			}

			TabStripItem* AddItem(TabStripItem* item) {
				
				GetChildren().Add(item);
				return item;

			}
			TabStripItem* AddItem(const String& text) {
				
				TabStripItem* item = new TabStripItem(text);
				AddItem(item);

				return item;

			}

			void OnUpdate(WidgetUpdateEventArgs& e) override {
				
				// Evenly space out all tabs over the width of the tab strip.
				float width = Width() / GetChildren().Count();
				float x_offset = 0.0f;

				for (auto i = GetChildren().GetWidgets().begin(); i != GetChildren().GetWidgets().end(); ++i) {
					i->widget->SetWidth(width);
					i->widget->SetHeight(Height());
					i->widget->SetX(x_offset);
					x_offset += width;
				}

			}

		};

	}
}