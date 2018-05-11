#pragma once
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetManager.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetBase : public IWidget {

			typedef std::unordered_map<WidgetEventType, std::function<void(IWidgetEventArgs&)>> callback_table_type;

		public:
			WidgetBase();
			WidgetBase(float x, float y, float width, float height);
			WidgetBase(const PointF& position, const SizeF& size);

			void HandleEvent(IWidgetEventArgs& ev) override;

			const std::string& Identifier() const override;
			void SetIdentifier(const std::string& value) override;
			const PointF& Position() const override;
			void SetPosition(const PointF& value) override;
			void SetPosition(float x, float y) override;
			float X() const override;
			float Y() const override;
			void SetX(float value) override;
			void SetY(float value) override;
			PointF FixedPosition() const override;
			const SizeF& Size() const override;
			void SetSize(const SizeF& value) override;
			void SetSize(float width, float height) override;
			float Width() const override;
			float Height() const override;
			void SetWidth(float value) override;
			void SetHeight(float value) override;
			const String& Text() const override;
			void SetText(const String& text) override;
			WidgetState State() const override;
			void SetState(WidgetState state, bool value) override;
			Gui::Anchor Anchor() const override;
			void SetAnchor(Gui::Anchor value) override;
			Gui::DockStyle DockStyle() const override;
			void SetDockStyle(Gui::DockStyle value) override;
			WidgetManager& GetChildren() override;
			bool HasChildren() override;
			IWidget* GetParent() override;
			void SetParent(IWidget* value) override;
			RectangleF Bounds() const override;
			bool Visible() const override;
			void SetVisible(bool value) override;
			SystemCursor Cursor() const override;
			void SetCursor(SystemCursor cursor) override;

			void BringToFront() override;
			void SendToBack() override;

			void OnMouseDown(WidgetMouseDownEventArgs& e) override;
			void OnMouseEnter(WidgetMouseEnterEventArgs& e) override;
			void OnMouseHover(WidgetMouseHoverEventArgs& e) override;
			void OnMouseLeave(WidgetMouseLeaveEventArgs& e) override;
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override;
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override;
			void OnMousePressed(WidgetMousePressedEventArgs& e) override;
			void OnUpdate(WidgetUpdateEventArgs& e) override;
			void OnManagerChanged(WidgetManagerChangedEventArgs& e) override;
			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override;
			void OnFocus(WidgetFocusEventArgs& e) override;
			void OnFocusLost(WidgetFocusLostEventArgs& e) override;
			void OnDraw(WidgetDrawEventArgs& e) override;
			void OnZDepthChanged(WidgetZDepthChangedEventArgs& e) override;

			template <WidgetEventType WIDGET_EVENT_TYPE>
			void SetEventHandler(const std::function<void(typename WidgetEventTypeTraits<WIDGET_EVENT_TYPE>::type&)>& callback) {
				_callbacks.emplace(WIDGET_EVENT_TYPE, [=](IWidgetEventArgs& x) {
					callback(reinterpret_cast<typename WidgetEventTypeTraits<WIDGET_EVENT_TYPE>::type&>(x));
				});
			}

		protected:
			WidgetManager* GetManager() const override;
			void SetManager(WidgetManager* value) override;

			void DoEventHandler(WidgetEventType ev, IWidgetEventArgs& args);

		private:
			std::string _name;
			PointF _position;
			SizeF _size;
			String _text;
			WidgetState _state;
			Gui::Anchor _anchor;
			Gui::DockStyle _dock_style;
			IWidget* _parent;
			bool _visible;
			SystemCursor _cursor;
			WidgetManager* _parent_manager;
			callback_table_type _callbacks;

		};

	}
}