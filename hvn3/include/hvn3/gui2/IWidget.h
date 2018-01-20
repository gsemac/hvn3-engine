#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"
#include "hvn3/gui2/WidgetEventArgs.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/utility/Size.h"
#include <string>

namespace hvn3 {
	namespace Gui {

		class WidgetManager;

		class IWidget {
			friend class WidgetManager;

		public:
			virtual ~IWidget() = default;
			
			// Dispatches the given event to the appropriate event handler.
			virtual void HandleEvent(WidgetEventArgs& ev) = 0;
			// Specifies a callback function to be called when handling the given event type.
			virtual void SetEventHandler(WidgetEvent ev, const EventHandler& callback) = 0;
			
			// Returns a string representing the widget's styling class hierarchy.
			virtual const std::string& Name() const = 0;
			virtual void SetName(const std::string& value) = 0;
			virtual const PointF& Position() const = 0;
			virtual void SetPosition(const PointF& value) = 0;
			virtual const SizeF& Size() const = 0;
			virtual void SetSize(const SizeF& value) = 0;

			virtual void OnMouseHover(MouseHoverEventArgs& e) = 0;

		protected:
			virtual WidgetManager* Manager() = 0;
			virtual void SetManager(WidgetManager* value) = 0;

		};

	}
}