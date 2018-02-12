#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"
#include "hvn3/gui2/WidgetEventArgs.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/utility/Size.h"
#include "hvn3/utility/Utf8String.h"
#include <string>

namespace hvn3 {
	namespace Gui {

		class WidgetManager;
		class WidgetStyle;

		class IWidget {
			friend class WidgetManager;

		public:
			virtual ~IWidget() = default;

			// Dispatches the given event to the appropriate event handler.
			virtual void HandleEvent(WidgetEventArgs& ev) = 0;

			// Returns a string representing the widget's styling class hierarchy.
			virtual const std::string& Name() const = 0;
			virtual void SetName(const std::string& value) = 0;
			virtual const PointF& Position() const = 0;
			virtual void SetPosition(const PointF& value) = 0;
			virtual const SizeF& Size() const = 0;
			virtual void SetSize(const SizeF& value) = 0;
			virtual const String& Text() const = 0;
			virtual void SetText(const String& text) = 0;
			virtual const WidgetStyle& Style() const = 0;
			virtual WidgetStyle& GetStyle() = 0;
			virtual void SetStyle(const WidgetStyle& value) = 0;

			virtual void OnMouseHover(WidgetMouseHoverEventArgs& e) = 0;
			virtual void OnUpdate(WidgetUpdateEventArgs& e) = 0;

		protected:
			virtual WidgetManager* Manager() = 0;
			virtual void SetManager(WidgetManager* value) = 0;

		};

	}
}