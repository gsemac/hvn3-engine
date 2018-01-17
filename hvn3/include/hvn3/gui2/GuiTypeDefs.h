#pragma once
#include <functional>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class WidgetEventArgs;

		typedef std::function<void(IWidget&, WidgetEventArgs&)> EventHandler;

		enum class ButtonState {
			Default,
			Pressed,
			Disabled
		};

	}
}