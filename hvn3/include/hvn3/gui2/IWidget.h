#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"
#include "hvn3/gui2/WidgetEventArgs.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/utility/Size.h"
#include "hvn3/utility/Utf8String.h"
#include <string>

namespace hvn3 {
	namespace Gui {

		class GuiManager;
		class WidgetManager;

		class IWidget {
			friend class WidgetManager;

		public:
			virtual ~IWidget() = default;

			// Dispatches the given event to the appropriate event handler.
			virtual void HandleEvent(WidgetEventArgs& ev) = 0;

			// Returns a string representing the widget's styling class hierarchy.
			virtual const std::string& Identifier() const = 0;
			virtual void SetIdentifier(const std::string& value) = 0;
			virtual const PointF& Position() const = 0;
			virtual void SetPosition(const PointF& value) = 0;
			virtual void SetPosition(float x, float y) = 0;
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

			virtual void OnMouseDown(WidgetMouseEventArgs& e) = 0;
			virtual void OnMouseEnter(WidgetMouseMoveEventArgs& e) = 0;
			virtual void OnMouseHover(WidgetMouseHoverEventArgs& e) = 0;
			virtual void OnMouseLeave(WidgetMouseMoveEventArgs& e) = 0;
			virtual void OnMouseMove(WidgetMouseMoveEventArgs& e) = 0;
			virtual void OnMouseUp(WidgetMouseEventArgs& e) = 0;
			virtual void OnUpdate(WidgetUpdateEventArgs& e) = 0;
			virtual void OnManagerChanged(WidgetManagerChangedEventArgs& e) = 0;
			virtual void OnRendererChanged(WidgetRendererChangedEventArgs& e) = 0;

		protected:
			virtual WidgetManager* GetManager() const = 0;
			virtual void SetManager(WidgetManager* value) = 0;

		};

	}
}