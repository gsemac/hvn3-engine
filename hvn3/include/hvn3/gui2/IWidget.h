#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"
#include "hvn3/gui2/WidgetEventArgs.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/utility/Size.h"
#include "hvn3/utility/Utf8String.h"
#include <string>

namespace hvn3 {
	namespace Gui {
	
		class ContextMenu;
		class GuiManager;
		class WidgetManager;
		class IWidgetRenderer;

		class IWidget {
			friend class WidgetManager;

		public:
			virtual ~IWidget() = default;

			// Dispatches the given event to the appropriate event handler.
			virtual void HandleEvent(IWidgetEventArgs& ev) = 0;

			// Returns a string representing the widget's styling class hierarchy.
			virtual const std::string& Id() const = 0;
			virtual void SetId(const std::string& value) = 0;
			virtual void AddId(const std::string& value) = 0;
			virtual const PointF& Position() const = 0;
			virtual void SetPosition(const PointF& value) = 0;
			virtual void SetPosition(float x, float y) = 0;
			virtual float X() const = 0;
			virtual float Y() const = 0;
			virtual void SetX(float value) = 0;
			virtual void SetY(float value) = 0;
			virtual PointF FixedPosition() const = 0;
			virtual const SizeF& Size() const = 0;
			virtual void SetSize(const SizeF& value) = 0;
			virtual void SetSize(float width, float height) = 0;
			virtual float Width() const = 0;
			virtual float Height() const = 0;
			virtual void SetWidth(float value) = 0;
			virtual void SetHeight(float value) = 0;
			virtual const String& Text() const = 0;
			virtual void SetText(const String& text) = 0;
			virtual WidgetState State() const = 0;
			virtual void SetState(WidgetState state, bool value) = 0;
			virtual Anchor Anchor() const = 0;
			virtual void SetAnchor(Gui::Anchor value) = 0;
			virtual Gui::DockStyle DockStyle() const = 0;
			virtual void SetDockStyle(Gui::DockStyle value) = 0;
			virtual WidgetManager& GetChildren() = 0;
			virtual bool HasChildren() = 0;
			virtual IWidget* GetParent() const = 0;
			virtual WidgetManager* GetManager() const = 0;
			virtual void SetParent(IWidget* value) = 0;
			virtual RectangleF Bounds() const = 0;
			virtual bool Visible() const = 0;
			virtual void SetVisible(bool value) = 0;
			virtual SystemCursor Cursor() const = 0;
			virtual void SetCursor(SystemCursor cursor) = 0;
			virtual bool TabStop() const = 0;
			virtual void SetTabStop(bool value) = 0;
			virtual bool HasFocus() const = 0;
			virtual void Focus() = 0;

			virtual ContextMenu* GetContextMenu() = 0;
			virtual void SetContextMenu(ContextMenu* context_menu) = 0;

			virtual void BringToFront() = 0;
			virtual void SendToBack() = 0;

			virtual void OnMouseDown(WidgetMouseDownEventArgs& e) = 0;
			virtual void OnMouseEnter(WidgetMouseEnterEventArgs& e) = 0;
			virtual void OnMouseHover(WidgetMouseHoverEventArgs& e) = 0;
			virtual void OnMouseLeave(WidgetMouseLeaveEventArgs& e) = 0;
			virtual void OnMouseMove(WidgetMouseMoveEventArgs& e) = 0;
			virtual void OnMousePressed(WidgetMousePressedEventArgs& e) = 0;
			virtual void OnMouseReleased(WidgetMouseReleasedEventArgs& e) = 0;
			virtual void OnUpdate(WidgetUpdateEventArgs& e) = 0;
			virtual void OnManagerChanged(WidgetManagerChangedEventArgs& e) = 0;
			virtual void OnRendererChanged(WidgetRendererChangedEventArgs& e) = 0;
			virtual void OnFocus(WidgetFocusEventArgs& e) = 0;
			virtual void OnFocusLost(WidgetFocusLostEventArgs& e) = 0;
			virtual void OnDraw(WidgetDrawEventArgs& e) = 0;
			virtual void OnZDepthChanged(WidgetZDepthChangedEventArgs& e) = 0;
			virtual void OnChildWidgetAdded(ChildWidgetAddedEventArgs& e) = 0;
			virtual void OnKeyDown(WidgetKeyDownEventArgs& e) = 0;
			virtual void OnKeyPressed(WidgetKeyPressedEventArgs& e) = 0;
			virtual void OnKeyUp(WidgetKeyUpEventArgs& e) = 0;
			virtual void OnKeyChar(WidgetKeyCharEventArgs& e) = 0;

		protected:
			virtual void SetManager(WidgetManager* value) = 0;
			virtual IWidgetRenderer* GetRenderer() const = 0;

		};

	}
}