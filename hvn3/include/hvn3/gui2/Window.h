#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Window : public WidgetBase {

		public:
			Window(float x, float y, float width, float height, const String& text);

			void OnMouseDown(WidgetMouseEventArgs& e) override;
			void OnMouseUp(WidgetMouseEventArgs& e) override;
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override;

		private:
			void _initializeMembers();
			
			PointF _mouse_down_position;
			bool _is_mouse_dragging;

		};

	}
}