#pragma once
#include <functional>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class WidgetEventArgs;

		enum class ButtonState {
			Default,
			Pressed,
			Disabled
		};

	}
}