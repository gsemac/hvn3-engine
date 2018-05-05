#include "hvn3/gui2/MenuStrip.h"

namespace hvn3 {
	namespace Gui {

		MenuStripItem::MenuStripItem(const std::string& text) {

			SetIdentifier("menustripitem");
			SetText(text);

		}
		void MenuStripItem::OnMouseUp(WidgetMouseEventArgs& e) {

			ContextMenu* cm = new ContextMenu();
			cm->SetPosition(Position().x, Position().y + Height());
			cm->AddItem("Context Menu Item 1");
			cm->AddItem("Context Menu Item 2");
			cm->AddItem("Context Menu Item 3");

			_owner->_showContextMenu(cm);

		}

		void MenuStripItem::_setOwner(MenuStrip* ms) {
			_owner = ms;
		}




		MenuStrip::MenuStrip() :
			MenuStrip(25.0f) {
		}
		MenuStrip::MenuStrip(float height) :
			_child_manager(this) {

			SetIdentifier("menustrip");
			SetDockStyle(DockStyle::Top);
			SetHeight(height);

			_active_item = nullptr;

		}
		void MenuStrip::AddItem(std::unique_ptr<IWidget>& item) {
			_child_manager.Add(item);
		}
		void MenuStrip::AddItem(MenuStripItem* item) {

			item->_setOwner(this);

			AddItem(std::unique_ptr<IWidget>(item));

		}
		WidgetManager& MenuStrip::GetChildren() {
			return _child_manager;
		}
		bool MenuStrip::HasChildren() {
			return _child_manager.Count() > 0;
		}
		void MenuStrip::OnUpdate(WidgetUpdateEventArgs& e) {
			WidgetBase::OnUpdate(e);

			// Update the dockable region for child widgets (in case this widget's dimensions have changed).
			_child_manager.SetDockableRegion(RectangleF(0.0f, 0.0f, Width(), Height()));

		}
		void MenuStrip::OnRendererChanged(WidgetRendererChangedEventArgs& e) {
			WidgetBase::OnRendererChanged(e);

			if (GetManager() == nullptr)
				return;

			// Resize all child widgets according to the new renderer.

			float margin_x = 2.0f;
			float margin_y = 1.0f;
			float padding = 4.0f;

			float x = margin_x;
			float y = margin_y;

			for (auto i = _child_manager.GetWidgets().begin(); i != _child_manager.GetWidgets().end(); ++i) {

				SizeF text_size = GetManager()->Renderer().MeasureString(i->widget->Text());

				i->widget->SetPosition(x, y);
				i->widget->SetSize(text_size.width + (padding * 2.0f), Height());

				x += margin_x + i->widget->Width();

			}

		}

		void MenuStrip::_showContextMenu(ContextMenu* cm) {

			if (GetManager() == nullptr)
				return;
			std::cout << "added context menu\n";
			GetManager()->Add(cm);

		}

	}
}