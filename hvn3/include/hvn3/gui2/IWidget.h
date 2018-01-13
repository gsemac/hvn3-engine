#pragma once
#include "hvn3/gui2/WidgetEventArgs.h"
#include <functional>

namespace hvn3 {
	namespace Gui {

		class IWidget {

		public:
			typedef std::function<void(IWidget&, WidgetEventArgs&)> event_handler_type;

			virtual ~IWidget() = default;
			
			// Dispatches the given event to the appropriate event handler.
			virtual void HandleEvent(WidgetEventArgs& ev) = 0;
			// Specifies a callback function to be called when handling the given event type.
			virtual void SetEventHandler(GuiEvent ev, const event_handler_type& callback) = 0;
			
		};

	}
}