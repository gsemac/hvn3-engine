#pragma once
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class MenuStripItem : public WidgetBase {

		public:
			MenuStripItem(const std::string& text) {

				SetIdentifier("menustripitem");
				SetText(text);

			}

		};

		class MenuStrip : public WidgetBase {

		public:
			MenuStrip() :
				MenuStrip(25.0f) {
			}
			MenuStrip(float height) {

				SetIdentifier("menustrip");
				SetDockStyle(DockStyle::Top);
				SetHeight(height);

			}

			void AddItem(std::unique_ptr<IWidget>& item) {
				_children.push_back(item.get());
				_child_manager.Add(item);
			}
			void AddItem(MenuStripItem* item) {
				AddItem(std::unique_ptr<IWidget>(item));
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

				// Resize all child widgets according to the new renderer.

				float margin_x = 2.0f;
				float margin_y = 1.0f;
				float padding = 4.0f;

				float x = margin_x;
				float y = margin_y;

				for (auto i = _children.begin(); i != _children.end(); ++i) {

					SizeF text_size = GetManager()->Renderer().MeasureString((*i)->Text());

					(*i)->SetPosition(x, y);
					(*i)->SetSize(text_size.width + (padding * 2.0f), Height());

					x += margin_x + (*i)->Width();

				}

			}

		private:
			std::list<IWidget*> _children;
			WidgetManager _child_manager;

		};

	}
}