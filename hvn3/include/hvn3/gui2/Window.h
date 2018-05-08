#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Window : public WidgetBase {

		public:
			Window(const String& text);
			Window(float x, float y, float width, float height, const String& text);

			void OnMouseDown(WidgetMouseDownEventArgs& e) override;
			void OnMouseUp(WidgetMouseUpEventArgs& e) override;
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override;

		private:
			void _initializeMembers();
			
			PointF _mouse_down_position;
			bool _is_mouse_dragging;

		};

	}
}