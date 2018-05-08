#pragma once
#include "hvn3/utility/BitFlags.h"
#include <functional>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class IWidgetEventArgs;

		enum class Anchor {
			Left = 1,
			Right = 2,
			Top = 4,
			Bottom = 8
		};
		ENABLE_BITFLAG_OPERATORS(Anchor)

		enum class DockStyle {
			Left = 1,
			Right,
			Top,
			Bottom,
			Fill
		};

		enum class WidgetProperty {
			BackgroundColor,
			Color,
			TextOffset,
			WIDGET_PROPERTY_MAX
		};

		enum class WidgetEventType {
			OnFocus,
			OnFocusLost,
			// Generated when the widget is clicked.
			OnMouseClick,
			OnMouseDown,
			OnMouseEnter,
			OnMouseHover,
			OnMouseLeave,
			OnMouseMove,
			OnMousePressed,
			OnMouseReleased,
			// Generated when the widget changes position.
			OnMove,
			OnUpdate,
			OnManagerChanged,
			OnRendererChanged
		};

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