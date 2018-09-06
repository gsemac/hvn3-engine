#pragma once
#include "hvn3/gui2/ScrollableWidgetBase.h"
#include "hvn3/gui2/ContainerWidgetBase.h"
#include <cassert>

namespace hvn3 {
	namespace Gui {


		class ListBoxItem :
			public WidgetBase {

		public:
			ListBoxItem() {

				SetId("listboxitem");
				SetHeight(15.0f);

			}
			ListBoxItem(const String& text) :
				ListBoxItem() {

				SetText(text);

			}

			void OnFocus(WidgetFocusEventArgs& e) {

				WidgetBase::OnFocus(e);

				bool already_selected = HasFlag(State(), WidgetState::Selected);

				SetState(WidgetState::Selected, true);

				if (GetManager() != nullptr) {

					int c_index = 0;
					int this_index = -1;

					// Un-select all other items.
					for (auto i = GetManager()->begin(); i != GetManager()->end(); ++i, ++c_index)
						if (i->widget.get() != this)
							i->widget->SetState(WidgetState::Selected, false);
						else
							this_index = c_index;

					// If the selection has changed, emit the appropriate event.
					if (!already_selected && GetParent() != nullptr)
						GetParent()->HandleEvent(WidgetSelectedItemChangedEventArgs(GetParent(), this, this_index));

				}

			}

		};

		class ListBox :
			public ScrollableWidgetBase,
			public ContainerWidgetBase {

		public:
			ListBox() :
				ScrollableWidgetBase(SizeF(0.0f, 0.0f)) {

				SetId("listbox");

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
			void SetSelectedIndex(int value) {

				ClearSelection();

				if (value == -1)
					return;

				assert(value >= 0);
				assert(static_cast<size_t>(value) < Count());

				int c_index = 0;

				for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i, ++c_index)
					if (c_index == value) {

						i->widget->SetState(WidgetState::Selected, true);

						EmitEvent(WidgetSelectedItemChangedEventArgs(this, i->widget.get(), value));

						break;

					}

			}
			IWidget* SelectedItem() {

				for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i)
					if (HasFlag(i->widget->State(), WidgetState::Selected))
						return i->widget.get();

				return nullptr;

			}

			void ClearSelection() {

				for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i)
					i->widget->SetState(WidgetState::Selected, false);

			}

			IWidget* ItemAt(int index) {

				if (index < 0 || static_cast<size_t>(index) > Count())
					return nullptr;

				int c_index = 0;

				for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i, ++c_index)
					if (c_index == index)
						return i->widget.get();

				return nullptr;

			}

			size_t Count() {

				return GetChildren().Count();

			}

			bool HasChildren() override {
				return ContainerWidgetBase::HasChildren();
			}
			WidgetManager& GetChildren() override {
				return ContainerWidgetBase::GetChildren();
			}
			void SetVisible(bool value) override {

				ContainerWidgetBase::SetVisible(value);
				ScrollableWidgetBase::SetVisible(value);
				WidgetBase::SetVisible(value);

			}

			void OnManagerChanged(WidgetManagerChangedEventArgs& e) override {

				ContainerWidgetBase::OnManagerChanged(e);
				ScrollableWidgetBase::OnManagerChanged(e);
				WidgetBase::OnManagerChanged(e);

			}
			void OnUpdate(WidgetUpdateEventArgs& e) override {

				ScrollableWidgetBase::OnUpdate(e);
				ContainerWidgetBase::OnUpdate(e);

				// Position and size all items.

				float yoff = 0.0f;

				for (auto i = GetChildren().GetWidgets().begin(); i != GetChildren().GetWidgets().end(); ++i) {

					i->widget->SetWidth(Width());
					i->widget->SetY(yoff - VisibleRegion().Y());

					yoff += i->widget->Height();

				}

				SetScrollableSize(SizeF(0.0f, yoff));

				GetChildren().SetDockableRegion(RectangleF(Size()));

			}

		};

	}
}