#include "hvn3/gui2/WidgetBase.h"
#define CAST_TO_EVENT_TYPE(TYPE, EV) reinterpret_cast<hvn3::Gui::GetWidgetEventType<TYPE>::type&>(EV)
#define EVENT_HANDLER_CASE(TYPE, CALLBACK, EV)\
case TYPE:\
	CALLBACK(CAST_TO_EVENT_TYPE(TYPE, EV));\
	break;

namespace hvn3 {
	namespace Gui {

		WidgetBase::WidgetBase(float x, float y, float width, float height) :
			WidgetBase(PointF(x, y), SizeF(width, height)) {
		}
		WidgetBase::WidgetBase(const PointF& position, const SizeF& size) :
			_position(position),
			_size(size) {

			_parent_manager = nullptr;

		}

		void WidgetBase::HandleEvent(WidgetEventArgs& ev) {

			DoEventHandler(ev.Type(), ev);

			if (ev.Handled())
				return;

			switch (ev.Type()) {
				EVENT_HANDLER_CASE(WidgetEventType::OnUpdate, OnUpdate, ev);
				EVENT_HANDLER_CASE(WidgetEventType::OnMouseHover, OnMouseHover, ev);
			}

		}

		const std::string& WidgetBase::Name() const {

			return _name;

		}
		void WidgetBase::SetName(const std::string& value) {

			_name = value;

		}
		const PointF& WidgetBase::Position() const {

			return _position;

		}
		void WidgetBase::SetPosition(const PointF& value) {

			_position = value;

		}
		const SizeF& WidgetBase::Size() const {

			return _size;

		}
		void WidgetBase::SetSize(const SizeF& value) {

			_size = value;

		}
		const String& WidgetBase::Text() const {
			return _text;
		}
		void WidgetBase::SetText(const String& text) {
			_text = text;
		}
		const WidgetStyle& WidgetBase::Style() const {
			return _style;
		}
		WidgetStyle& WidgetBase::GetStyle() {
			return _style;
		}
		void WidgetBase::SetStyle(const WidgetStyle& value) {
			_style = WidgetStyle(value);
		}

		void WidgetBase::OnMouseHover(WidgetMouseHoverEventArgs& e) {}
		void WidgetBase::OnUpdate(WidgetUpdateEventArgs& e) {}


		WidgetManager* WidgetBase::Manager() {

			return _parent_manager;

		}
		void WidgetBase::SetManager(WidgetManager* value) {

			_parent_manager = value;

			if (value != nullptr)
				_child_control_manager.SetRenderer(_parent_manager->GetRenderer());

		}

		void WidgetBase::DoEventHandler(WidgetEventType ev, WidgetEventArgs& args) {

			auto iter = _callbacks.find(ev);

			if (iter == _callbacks.end())
				return;

			iter->second(args);

		}

	}
}