#pragma once
#include "hvn3/utility/BitFlags.h"

#include <functional>
#include <memory>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class IWidgetEventArgs;

		typedef std::shared_ptr<IWidget> IWidgetPtr;
		template <typename WidgetType>
		using WidgetPtr = std::shared_ptr<WidgetType>;

		enum class Anchor {
			Left = 1,
			Right = 2,
			Top = 4,
			Bottom = 8
		};
		ENABLE_BITFLAG_OPERATORS(Anchor);

		enum class DockStyle {
			None = 0,
			Left = 1,
			Right,
			Top,
			Bottom,
			Fill
		};

		enum class Orientation {
			Horizontal,
			Vertical
		};

		enum class WidgetProperty {
			BackgroundColor,
			BackgroundImage,
			BackgroundPosition,
			Color,
			TextOffset,
			WIDGET_PROPERTY_MAX
		};

		enum class WidgetEventType {
			OnDraw,
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
			OnMouseScroll,
			// Generated when the widget changes position.
			OnMove,
			OnUpdate,
			OnManagerChanged,
			OnRendererChanged,
			OnZDepthChanged,
			OnChildWidgetAdded,
			OnSelectedTabChanged,
			OnKeyDown,
			OnKeyPressed,
			OnKeyReleased,
			OnKeyChar,
			OnCheckedStateChanged,
			OnTextChanged,
			OnSelectedItemChanged
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
			Selected = 4096,
			Valid = 8192
		};
		ENABLE_BITFLAG_OPERATORS(WidgetState);

		enum class InputType {
			Default,
			Numeric,
			Decimal
		};

		template<typename WidgetType, typename... Args>
		WidgetPtr<WidgetType> make_widget(Args&&... args) {

			WidgetPtr<WidgetType> ptr = std::make_shared<WidgetType>(std::forward<Args>(args)...);

			return ptr;

		}

	}
}