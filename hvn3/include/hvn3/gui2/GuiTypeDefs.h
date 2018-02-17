#pragma once
#include "hvn3/utility/BitFlags.h"
#include <functional>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class WidgetEventArgs;

		enum class WidgetState {
			Active = 1,
			Checked = 2,
			Disabled = 4,
			Enabled = 8,
			Focus = 16,
			Hover = 32,
			Invalid = 64,
			Optional = 128,
			OutOfRange = 256,
			ReadOnly = 512,
			ReadWrite = 1024,
			Required = 2048,
			Valid = 4096
		};
		ENABLE_BITFLAG_OPERATORS(WidgetState)

	}
}