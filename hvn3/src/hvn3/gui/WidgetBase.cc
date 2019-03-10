//#include "hvn3/gui/ContextMenu.h"
//#include "hvn3/gui/WidgetBase.h"
//#include "hvn3/gui/IWidgetRenderer.h"
//
//#define CAST_TO_EVENT_TYPE(TYPE, EV) reinterpret_cast<hvn3::Gui::WidgetEventTypeTraits<TYPE>::type&>(EV)
//#define EVENT_HANDLER_CASE(TYPE, CALLBACK, EV)\
//case TYPE:\
//	CALLBACK(CAST_TO_EVENT_TYPE(TYPE, EV));\
//	break;
//
//namespace hvn3 {
//	namespace Gui {
//
//		WidgetBase::WidgetBase() :
//			WidgetBase(0.0f, 0.0f, 0.0f, 0.0f) {
//		}
//		WidgetBase::WidgetBase(float x, float y, float width, float height) :
//			WidgetBase(PointF(x, y), SizeF(width, height)) {
//		}
//		WidgetBase::WidgetBase(const PointF& position, const SizeF& size) :
//			_position(position),
//			_size(size) {
//
//			_anchor = static_cast<Gui::Anchor>(0);
//			_dock_style = static_cast<Gui::DockStyle>(0);
//			_parent_manager = nullptr;
//			_parent = nullptr;
//			_visible = true;
//			_cursor = SystemCursor::Default;
//			_tab_stop = true;
//			_key_preview_enabled = false;
//
//			_context_menu = nullptr;
//			_context_menu_managed = false;
//
//		}
//
//		void WidgetBase::HandleEvent(IWidgetEventArgs& ev) {
//
//			DoEventHandler(ev.Type(), ev);
//
//			if (ev.Handled())
//				return;
//
//			switch (ev.Type()) {
//				EVENT_HANDLER_CASE(WidgetEventType::OnUpdate, OnUpdate, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseDown, OnMouseDown, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseEnter, OnMouseEnter, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseHover, OnMouseHover, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseLeave, OnMouseLeave, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseMove, OnMouseMove, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseReleased, OnMouseReleased, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMousePressed, OnMousePressed, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnFocus, OnFocus, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnFocusLost, OnFocusLost, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnKeyDown, OnKeyDown, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnKeyPressed, OnKeyPressed, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnKeyReleased, OnKeyUp, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnKeyChar, OnKeyChar, ev);
//				EVENT_HANDLER_CASE(WidgetEventType::OnMouseScroll, OnMouseScroll, ev);
//			}
//
//		}
//
//		const std::string& WidgetBase::Id() const {
//			return _name;
//		}
//		void WidgetBase::SetId(const std::string& value) {
//			_name = value;
//		}
//		void WidgetBase::AddId(const std::string& value) {
//
//			_name.push_back(' ');
//			_name += value;
//
//		}
//		const PointF& WidgetBase::Position() const {
//
//			return _position;
//
//		}
//		void WidgetBase::SetPosition(const PointF& value) {
//
//			_position = value;
//
//		}
//		void WidgetBase::SetPosition(float x, float y) {
//			SetPosition(PointF(x, y));
//		}
//		float WidgetBase::X() const {
//			return Position().x;
//		}
//		float WidgetBase::Y() const {
//			return Position().y;
//		}
//		void WidgetBase::SetX(float value) {
//			SetPosition(value, Y());
//		}
//		void WidgetBase::SetY(float value) {
//			SetPosition(X(), value);
//		}
//		PointF WidgetBase::FixedPosition() const {
//
//			PointF position = Position();
//
//			if (_parent != nullptr)
//				position += _parent->FixedPosition();
//
//			if (GetManager() != nullptr)
//				position += GetManager()->DockableRegion().Position();
//
//			return position;
//
//		}
//		const SizeF& WidgetBase::Size() const {
//
//			return _size;
//
//		}
//		void WidgetBase::SetSize(const SizeF& value) {
//
//			_size = value;
//
//		}
//		void WidgetBase::SetSize(float width, float height) {
//			SetSize(SizeF(width, height));
//		}
//		float WidgetBase::Width() const {
//			return Size().width;
//		}
//		float WidgetBase::Height() const {
//			return Size().height;
//		}
//		void WidgetBase::SetWidth(float value) {
//			SetSize(SizeF(value, Size().height));
//		}
//		void WidgetBase::SetHeight(float value) {
//			SetSize(SizeF(Size().width, value));
//		}
//		const String& WidgetBase::Text() const {
//			return _text;
//		}
//		void WidgetBase::SetText(const String& text) {
//
//			_text = text;
//
//			EmitEvent(WidgetTextChangedEventArgs(this));
//
//		}
//		WidgetState WidgetBase::State() const {
//			return _state;
//		}
//		void WidgetBase::SetState(WidgetState state, bool value) {
//
//			if (value)
//				_state |= state;
//			else
//				_state &= ~state;
//
//		}
//		Gui::Anchor WidgetBase::Anchor() const {
//			return _anchor;
//		}
//		void WidgetBase::SetAnchor(Gui::Anchor value) {
//			_anchor = value;
//		}
//		Gui::DockStyle WidgetBase::DockStyle() const {
//			return _dock_style;
//		}
//		void WidgetBase::SetDockStyle(Gui::DockStyle value) {
//
//			_dock_style = value;
//
//			if (value == Gui::DockStyle::Top)
//				SetAnchor(Gui::Anchor::Top | Gui::Anchor::Right | Gui::Anchor::Left);
//			else if (value == Gui::DockStyle::Bottom)
//				SetAnchor(Gui::Anchor::Bottom | Gui::Anchor::Right | Gui::Anchor::Left);
//			else if (value == Gui::DockStyle::Left)
//				SetAnchor(Gui::Anchor::Left | Gui::Anchor::Top | Gui::Anchor::Bottom);
//			else if (value == Gui::DockStyle::Right)
//				SetAnchor(Gui::Anchor::Right | Gui::Anchor::Top | Gui::Anchor::Bottom);
//			else if (value == Gui::DockStyle::Fill)
//				SetAnchor(Gui::Anchor::Right | Gui::Anchor::Left | Gui::Anchor::Top | Gui::Anchor::Bottom);
//
//		}
//		WidgetManager& WidgetBase::GetChildren() {
//			throw System::NotSupportedException("This widget does not support child widgets.");
//		}
//		bool WidgetBase::HasChildren() {
//			return false;
//		}
//		IWidget* WidgetBase::GetParent() const {
//			return _parent;
//		}
//		void WidgetBase::SetParent(IWidget* value) {
//			_parent = value;
//		}
//		RectangleF WidgetBase::Bounds() const {
//			return RectangleF(FixedPosition(), Size());
//		}
//		bool WidgetBase::Visible() const {
//			return _visible;
//		}
//		void WidgetBase::SetVisible(bool value) {
//
//			_visible = value;
//
//			// If this widget is focused, clear focus before making it invisible.
//
//			if (_visible && HasFocus() && GetManager() != nullptr)
//				GetManager()->SetFocus(nullptr);
//
//			// If this widget has children, set the visibility of all children.
//
//			if (HasChildren())
//				for (auto i = GetChildren().begin(); i != GetChildren().end(); ++i)
//					i->widget->SetVisible(value);
//
//		}
//		SystemCursor WidgetBase::Cursor() const {
//			return _cursor;
//		}
//		void WidgetBase::SetCursor(SystemCursor cursor) {
//			_cursor = cursor;
//		}
//		bool WidgetBase::TabStop() const {
//			return _tab_stop;
//		}
//		void WidgetBase::SetTabStop(bool value) {
//			_tab_stop = value;
//		}
//		bool WidgetBase::HasFocus() const {
//			return HasFlag(State(), WidgetState::Focus);
//		}
//		void WidgetBase::Focus() {
//
//			if (_parent_manager == nullptr)
//				return;
//
//			_parent_manager->SetFocus(this);
//
//		}
//		bool WidgetBase::KeyPreviewEnabled() const {
//
//			return _key_preview_enabled;
//
//		}
//		void WidgetBase::SetKeyPreviewEnabled(bool value) {
//
//			_key_preview_enabled = value;
//
//		}
//
//		class ContextMenu* WidgetBase::GetContextMenu() {
//
//			return _context_menu;
//
//		}
//		void WidgetBase::SetContextMenu(class ContextMenu* context_menu) {
//
//			_freeContextMenu();
//
//			if (context_menu == nullptr)
//				return;
//
//			_context_menu = context_menu;
//			_context_menu->SetVisible(false);
//
//			_context_menu_managed = false;
//
//		}
//
//		void WidgetBase::BringToFront() {
//
//			if (GetManager() != nullptr)
//				GetManager()->BringToFront(this);
//
//		}
//		void WidgetBase::SendToBack() {
//
//			if (GetManager() != nullptr)
//				GetManager()->SendToBack(this);
//
//		}
//
//		void WidgetBase::OnMouseDown(WidgetMouseDownEventArgs& e) {
//			//SetState(WidgetState::Active, true);
//		}
//		void WidgetBase::OnMouseEnter(WidgetMouseEnterEventArgs& e) {
//			SetState(WidgetState::Hover, true);
//		}
//		void WidgetBase::OnMouseHover(WidgetMouseHoverEventArgs& e) {}
//		void WidgetBase::OnMouseLeave(WidgetMouseLeaveEventArgs& e) {
//			SetState(WidgetState::Hover, false);
//		}
//		void WidgetBase::OnMouseMove(WidgetMouseMoveEventArgs& e) {}
//		void WidgetBase::OnMouseReleased(WidgetMouseReleasedEventArgs& e) {
//
//			SetState(WidgetState::Active, false);
//
//			// If a context menu has been assigned, show it now.
//
//			if (e.Button() == MouseButton::Right)
//				ShowContextMenu(e.Position());
//
//		}
//		void WidgetBase::OnMousePressed(WidgetMousePressedEventArgs& e) {
//
//			SetState(WidgetState::Active, true);
//
//			// If the context menu is visible, hide it.
//			// It will disappear when this widget loses focus, but it also needs to disappear when the widget is clicked anywhere else.
//
//			_setContextMenuVisible(false);
//
//		}
//		void WidgetBase::OnUpdate(WidgetUpdateEventArgs& e) {}
//		void WidgetBase::OnManagerChanged(WidgetManagerChangedEventArgs& e) {}
//		void WidgetBase::OnRendererChanged(WidgetRendererChangedEventArgs& e) {}
//		void WidgetBase::OnFocus(WidgetFocusEventArgs& e) {}
//		void WidgetBase::OnFocusLost(WidgetFocusLostEventArgs& e) {
//
//			// If the context menu or its children received focus, postpone closing the context menu until the next mouse-up.
//			// This gives events like OnMouseClick a chance to be detected before the menu disappears.
//
//			if (!_isContextMenuOrContextMenuItemFocused())
//				_setContextMenuVisible(false);
//
//		}
//		void WidgetBase::OnDraw(WidgetDrawEventArgs& e) {}
//		void WidgetBase::OnZDepthChanged(WidgetZDepthChangedEventArgs& e) {}
//		void WidgetBase::OnChildWidgetAdded(ChildWidgetAddedEventArgs& e) {}
//		void WidgetBase::OnKeyDown(WidgetKeyDownEventArgs& e) {}
//		void WidgetBase::OnKeyPressed(WidgetKeyPressedEventArgs& e) {}
//		void WidgetBase::OnKeyUp(WidgetKeyUpEventArgs& e) {}
//		void WidgetBase::OnKeyChar(WidgetKeyCharEventArgs& e) {}
//		void WidgetBase::OnMouseScroll(WidgetMouseScrollEventArgs& e) {}
//
//
//		WidgetManager* WidgetBase::GetManager() const {
//			return _parent_manager;
//		}
//		void WidgetBase::SetManager(WidgetManager* value) {
//
//			WidgetManager* old_manager = _parent_manager;
//
//			_parent_manager = value;
//
//			OnManagerChanged(WidgetManagerChangedEventArgs(this, old_manager));
//			OnRendererChanged(WidgetRendererChangedEventArgs(this));
//
//			//if (value != nullptr)
//			//	_child_control_manager.SetRenderer(_parent_manager->GetRenderer());
//
//		}
//		IWidgetRenderer* WidgetBase::GetRenderer() const {
//
//			if (GetManager() == nullptr)
//				return nullptr;
//
//			return const_cast<const WidgetManager*>(GetManager())->Renderer().get();
//
//		}
//
//		void WidgetBase::DoEventHandler(WidgetEventType ev, IWidgetEventArgs& args) {
//
//			auto iter = _callbacks.find(ev);
//
//			if (iter == _callbacks.end())
//				return;
//
//			iter->second(args);
//
//		}
//		void WidgetBase::EmitEvent(IWidgetEventArgs& ev) {
//
//			// Having this function separate from HandleEvent allows more flexibility in the future (e.g., not handling events immediately).
//			// In the meantime, it's just an alias for HandleEvent.
//
//			HandleEvent(ev);
//
//		}
//
//		void WidgetBase::ShowContextMenu(const PointF& position) {
//
//			if (_context_menu == nullptr)
//				return;
//
//			_setContextMenuVisible(true);
//
//			_context_menu->SetPosition(PointF(Math::Max(0.0f, position.x), Math::Max(0.0f, position.y)));
//
//			// Make sure that the context menu is completely within the visible region.
//
//			WidgetManager* manager = _context_menu->GetManager();
//
//			if (manager != nullptr) {
//
//				RectangleF bounds = _context_menu->Bounds();
//				RectangleF region = manager->DockableRegion();
//
//				float x_over = bounds.Right() - region.Right();
//				float y_over = bounds.Bottom() - region.Bottom();
//
//				if (x_over > 0.0f)
//					_context_menu->SetX(_context_menu->X() - x_over);
//
//				if (y_over > 0.0f)
//					_context_menu->SetY(_context_menu->Y() - y_over);
//
//			}
//
//		}
//
//
//
//		WidgetManager* WidgetBase::_getTopmostManager() const {
//
//			const IWidget* ptr = this;
//
//			while (ptr->GetParent() != nullptr)
//				ptr = ptr->GetParent();
//
//			return ptr->GetManager();
//
//		}
//		void WidgetBase::_setContextMenuVisible(bool value) {
//
//			if (value) {
//
//				WidgetManager* manager = _getTopmostManager();
//
//				if (!_context_menu_managed && _context_menu != nullptr && manager != nullptr) {
//					manager->Add(_context_menu);
//					_context_menu_managed = true;
//				}
//
//				if (_context_menu != nullptr) {
//
//					PointF pos = Bounds().Position();
//
//					_context_menu->SetPosition(pos.x, pos.y + Height());
//					_context_menu->SetVisible(true);
//
//					if (manager != nullptr)
//						manager->BringToFront(_context_menu);
//
//				}
//
//			}
//			else {
//
//				if (_context_menu == nullptr)
//					return;
//
//				_context_menu->SetVisible(false);
//
//			}
//
//		}
//		void WidgetBase::_freeContextMenu() {
//
//			if (_context_menu == nullptr)
//				return;
//
//			// If the context menu isn't currently managed, just delete it.
//			if (!_context_menu_managed) {
//				delete _context_menu;
//				_context_menu = nullptr;
//			}
//
//			WidgetManager* manager = _getTopmostManager();
//
//			if (manager == nullptr)
//				return;
//
//			manager->Remove(_context_menu);
//
//			_context_menu = nullptr;
//
//		}
//		bool WidgetBase::_isContextMenuVisible() const {
//
//			if (_context_menu == nullptr)
//				return false;
//
//			return _context_menu->Visible();
//
//		}
//		bool WidgetBase::_isContextMenuOrContextMenuItemFocused() const {
//
//			if (!_isContextMenuVisible())
//				return false;
//
//			if (HasFlag(_context_menu->State(), WidgetState::Focus))
//				return true;
//
//			if (_context_menu->HasChildren())
//				for (auto i = _context_menu->GetChildren().begin(); i != _context_menu->GetChildren().end(); ++i)
//					if (HasFlag(i->widget->State(), WidgetState::Focus))
//						return true;
//
//			return false;
//
//		}
//
//	}
//}