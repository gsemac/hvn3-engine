#pragma once
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetManager.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetBase : public IWidget {

			typedef std::unordered_map<WidgetEventType, std::function<void(WidgetEventArgs&)>> callback_table_type;

		public:
			WidgetBase(float x, float y, float width, float height);
			WidgetBase(const PointF& position, const SizeF& size);

			void HandleEvent(WidgetEventArgs& ev) override;

			const std::string& Identifier() const override;
			void SetIdentifier(const std::string& value) override;
			const PointF& Position() const override;
			void SetPosition(const PointF& value) override;
			const SizeF& Size() const override;
			void SetSize(const SizeF& value) override;
			const String& Text() const override;
			void SetText(const String& text) override;
			virtual WidgetState State() const override;
			virtual void SetState(WidgetState state, bool value) override;

			void OnMouseDown(WidgetMouseEventArgs& e) override;
			void OnMouseEnter(WidgetMouseEventArgs& e) override;
			void OnMouseHover(WidgetMouseHoverEventArgs& e) override;
			void OnMouseLeave(WidgetMouseEventArgs& e) override;
			void OnMouseUp(WidgetMouseEventArgs& e) override;
			void OnUpdate(WidgetUpdateEventArgs& e) override;

			template <WidgetEventType WIDGET_EVENT_TYPE>
			void SetEventHandler(const std::function<void(typename GetWidgetEventType<WIDGET_EVENT_TYPE>::type&)>& callback) {
				_callbacks.emplace(WIDGET_EVENT_TYPE, [=](WidgetEventArgs& x) {
					callback(reinterpret_cast<typename GetWidgetEventType<WIDGET_EVENT_TYPE>::type&>(x));
				});
			}

		protected:
			WidgetManager* Manager() override;
			void SetManager(WidgetManager* value) override;

			void DoEventHandler(WidgetEventType ev, WidgetEventArgs& args);

		private:
			std::string _name;
			PointF _position;
			SizeF _size;
			String _text;
			WidgetState _state;
			WidgetManager* _parent_manager;
			WidgetManager _child_control_manager;
			callback_table_type _callbacks;

		};

	}
}