#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/gui2/IWidgetRenderer.h"

#define CAST_TO_EVENT_TYPE(TYPE, EV) reinterpret_cast<hvn3::Gui::WidgetEventTypeTraits<TYPE>::type&>(EV)
#define EVENT_HANDLER_CASE(TYPE, CALLBACK, EV)\
case TYPE:\
	CALLBACK(CAST_TO_EVENT_TYPE(TYPE, EV));\
	break;

namespace hvn3 {
	namespace Gui {

		WidgetBase::WidgetBase() :
			WidgetBase(0.0f, 0.0f, 0.0f, 0.0f) {
		}
		WidgetBase::WidgetBase(float x, float y, float width, float height) :
			WidgetBase(PointF(x, y), SizeF(width, height)) {
		}
		WidgetBase::WidgetBase(const PointF& position, const SizeF& size) :
			_position(position),
			_size(size) {

			_anchor = static_cast<Gui::Anchor>(0);
			_dock_style = static_cast<Gui::DockStyle>(0);
			_parent_manager = nullptr;
			_parent = nullptr;
			_visible = true;
			_cursor = SystemCursor::Default;

		}

		void WidgetBase::HandleEvent(IWidgetEventArgs& ev) {

			DoEventHandler(ev.Type(), ev);
			if (ev.Type() == WidgetEventType::OnKeyChar)
				
			if (ev.Handled())
				return;

			switch (ev.Type()) {
				EVENT_HANDLER_CASE(WidgetEventType::OnUpdate, OnUpdate, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseDown, OnMouseDown, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseEnter, OnMouseEnter, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseHover, OnMouseHover, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseLeave, OnMouseLeave, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseMove, OnMouseMove, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseReleased, OnMouseReleased, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMousePressed, OnMousePressed, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnFocus, OnFocus, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnFocusLost, OnFocusLost, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnKeyDown, OnKeyDown, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnKeyPressed, OnKeyPressed, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnKeyReleased, OnKeyUp, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnKeyChar, OnKeyChar, ev);
			}

		}

		const std::string& WidgetBase::Identifier() const {
			return _name;
		}
		void WidgetBase::SetIdentifier(const std::string& value) {
			_name = value;
		}
		const PointF& WidgetBase::Position() const {

			return _position;

		}
		void WidgetBase::SetPosition(const PointF& value) {

			_position = value;

		}
		void WidgetBase::SetPosition(float x, float y) {
			SetPosition(PointF(x, y));
		}
		float WidgetBase::X() const {
			return Position().x;
		}
		float WidgetBase::Y() const {
			return Position().y;
		}
		void WidgetBase::SetX(float value) {
			SetPosition(value, Y());
		}
		void WidgetBase::SetY(float value) {
			SetPosition(X(), value);
		}
		PointF WidgetBase::FixedPosition() const {

			PointF position = Position();

			if (_parent != nullptr)
				position += _parent->FixedPosition();

			if (GetManager() != nullptr)
				position += GetManager()->DockableRegion().Position();

			return position;

		}
		const SizeF& WidgetBase::Size() const {

			return _size;

		}
		void WidgetBase::SetSize(const SizeF& value) {

			_size = value;

		}
		void WidgetBase::SetSize(float width, float height) {
			SetSize(SizeF(width, height));
		}
		float WidgetBase::Width() const {
			return Size().width;
		}
		float WidgetBase::Height() const {
			return Size().height;
		}
		void WidgetBase::SetWidth(float value) {
			SetSize(SizeF(value, Size().height));
		}
		void WidgetBase::SetHeight(float value) {
			SetSize(SizeF(Size().width, value));
		}
		const String& WidgetBase::Text() const {
			return _text;
		}
		void WidgetBase::SetText(const String& text) {
			_text = text;
		}
		WidgetState WidgetBase::State() const {
			return _state;
		}
		void WidgetBase::SetState(WidgetState state, bool value) {
			if (value)
				_state |= state;
			else
				_state &= ~state;
		}
		Gui::Anchor WidgetBase::Anchor() const {
			return _anchor;
		}
		void WidgetBase::SetAnchor(Gui::Anchor value) {
			_anchor = value;
		}
		Gui::DockStyle WidgetBase::DockStyle() const {
			return _dock_style;
		}
		void WidgetBase::SetDockStyle(Gui::DockStyle value) {

			_dock_style = value;

			if (value == Gui::DockStyle::Top)
				SetAnchor(Gui::Anchor::Top | Gui::Anchor::Right | Gui::Anchor::Left);
			else if (value == Gui::DockStyle::Bottom)
				SetAnchor(Gui::Anchor::Bottom | Gui::Anchor::Right | Gui::Anchor::Left);
			else if (value == Gui::DockStyle::Left)
				SetAnchor(Gui::Anchor::Left | Gui::Anchor::Top | Gui::Anchor::Bottom);
			else if (value == Gui::DockStyle::Right)
				SetAnchor(Gui::Anchor::Right | Gui::Anchor::Top | Gui::Anchor::Bottom);
			else if (value == Gui::DockStyle::Fill)
				SetAnchor(Gui::Anchor::Right | Gui::Anchor::Left | Gui::Anchor::Top | Gui::Anchor::Bottom);

		}
		WidgetManager& WidgetBase::GetChildren() {
			throw System::NotSupportedException("This widget does not support child widgets.");
		}
		bool WidgetBase::HasChildren() {
			return false;
		}
		IWidget* WidgetBase::GetParent() {
			return _parent;
		}
		void WidgetBase::SetParent(IWidget* value) {
			_parent = value;
		}
		RectangleF WidgetBase::Bounds() const {
			return RectangleF(FixedPosition(), Size());
		}
		bool WidgetBase::Visible() const {
			return _visible;
		}
		void WidgetBase::SetVisible(bool value) {
			_visible = value;
		}
		SystemCursor WidgetBase::Cursor() const {
			return _cursor;
		}
		void WidgetBase::SetCursor(SystemCursor cursor) {
			_cursor = cursor;
		}

		void WidgetBase::BringToFront() {

			if (GetManager() != nullptr)
				GetManager()->BringToFront(this);

		}
		void WidgetBase::SendToBack() {

			if (GetManager() != nullptr)
				GetManager()->SendToBack(this);

		}

		void WidgetBase::OnMouseDown(WidgetMouseDownEventArgs& e) {
			//SetState(WidgetState::Active, true);
		}
		void WidgetBase::OnMouseEnter(WidgetMouseEnterEventArgs& e) {
			SetState(WidgetState::Hover, true);
		}
		void WidgetBase::OnMouseHover(WidgetMouseHoverEventArgs& e) {}
		void WidgetBase::OnMouseLeave(WidgetMouseLeaveEventArgs& e) {
			SetState(WidgetState::Hover, false);
		}
		void WidgetBase::OnMouseMove(WidgetMouseMoveEventArgs& e) {}
		void WidgetBase::OnMouseReleased(WidgetMouseReleasedEventArgs& e) {
			SetState(WidgetState::Active, false);
		}
		void WidgetBase::OnMousePressed(WidgetMousePressedEventArgs& e) {
			SetState(WidgetState::Active, true);
		}
		void WidgetBase::OnUpdate(WidgetUpdateEventArgs& e) {}
		void WidgetBase::OnManagerChanged(WidgetManagerChangedEventArgs& e) {}
		void WidgetBase::OnRendererChanged(WidgetRendererChangedEventArgs& e) {}
		void WidgetBase::OnFocus(WidgetFocusEventArgs& e) {
			SetState(WidgetState::Focus, true);
		}
		void WidgetBase::OnFocusLost(WidgetFocusLostEventArgs& e) {
			SetState(WidgetState::Focus, false);
		}
		void WidgetBase::OnDraw(WidgetDrawEventArgs& e) {}
		void WidgetBase::OnZDepthChanged(WidgetZDepthChangedEventArgs& e) {}
		void WidgetBase::OnChildWidgetAdded(ChildWidgetAddedEventArgs& e) {}
		void WidgetBase::OnKeyDown(WidgetKeyDownEventArgs& e) {}
		void WidgetBase::OnKeyPressed(WidgetKeyPressedEventArgs& e) {}
		void WidgetBase::OnKeyUp(WidgetKeyUpEventArgs& e) {}
		void WidgetBase::OnKeyChar(WidgetKeyCharEventArgs& e) {}



		WidgetManager* WidgetBase::GetManager() const {
			return _parent_manager;
		}
		void WidgetBase::SetManager(WidgetManager* value) {

			WidgetManager* old_manager = _parent_manager;

			_parent_manager = value;

			OnManagerChanged(WidgetManagerChangedEventArgs(this, old_manager));
			OnRendererChanged(WidgetRendererChangedEventArgs(this));

			//if (value != nullptr)
			//	_child_control_manager.SetRenderer(_parent_manager->GetRenderer());

		}
		IWidgetRenderer* WidgetBase::GetRenderer() const {
			
			if (GetManager() == nullptr)
				return nullptr;

			return GetManager()->GetRenderer().get();

		}

		void WidgetBase::DoEventHandler(WidgetEventType ev, IWidgetEventArgs& args) {

			auto iter = _callbacks.find(ev);

			if (iter == _callbacks.end())
				return;

			iter->second(args);

		}

	}
}