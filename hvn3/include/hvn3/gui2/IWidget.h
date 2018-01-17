#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"
#include "hvn3/gui2/WidgetEventArgs.h"
#include <string>

namespace hvn3 {
	namespace Gui {

		class IWidget {

		public:
			virtual ~IWidget() = default;
			
			// Dispatches the given event to the appropriate event handler.
			virtual void HandleEvent(WidgetEventArgs& ev) = 0;
			// Specifies a callback function to be called when handling the given event type.
			virtual void SetEventHandler(GuiEvent ev, const EventHandler& callback) = 0;
			
			// Returns a string representing the widget's styling class hierarchy.
			virtual const std::string& Name() const = 0;
			virtual void SetName(const std::string& value) = 0;
			virtual const PointF& Position() const = 0;
			virtual void SetPosition(const PointF& value) = 0;
			virtual const SizeF& Size() const = 0;
			virtual float SetSize(const SizeF& value) = 0;

		};

	}
}