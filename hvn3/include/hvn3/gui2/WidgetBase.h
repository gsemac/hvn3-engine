#pragma once
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetManager.h"
#include "hvn3/gui2/WidgetStyle.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetBase : public IWidget {

			typedef std::unordered_map<WidgetEventType, std::function<void(WidgetEventArgs&)>> callback_table_type;

		public:
			WidgetBase(float x, float y, float width, float height);
			WidgetBase(const PointF& position, const SizeF& size);

			void HandleEvent(WidgetEventArgs& ev) override;

			const std::string& Name() const override;
			void SetName(const std::string& value) override;
			const PointF& Position() const override;
			void SetPosition(const PointF& value) override;
			const SizeF& Size() const override;
			void SetSize(const SizeF& value) override;
			const String& Text() const override;
			void SetText(const String& text) override;
			const WidgetStyle& Style() const override;
			WidgetStyle& GetStyle() override;
			void SetStyle(const WidgetStyle& value) override;

			void OnMouseHover(WidgetMouseHoverEventArgs& e) override;

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
			WidgetStyle _style;
			WidgetManager* _parent_manager;
			WidgetManager _child_control_manager;
			callback_table_type _callbacks;

		};

	}
}