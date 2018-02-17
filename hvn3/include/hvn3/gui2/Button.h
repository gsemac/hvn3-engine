#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Button : public WidgetBase {

		public:
			Button(float x, float y, const String& text);
			Button(const PointF& position, const String& text);
			Button(float x, float y, float width, float height, const String& text);
			Button(const PointF& position, const SizeF& size, const const String& text);
			
			void OnUpdate(WidgetUpdateEventArgs& e) override;
			void OnMouseDown(WidgetMouseEventArgs& e) override;
			void OnMouseHover(WidgetMouseHoverEventArgs& e) override;
			void OnMouseUp(WidgetMouseEventArgs& e) override;

		private:
			Button();

			void _initializeMembers();

			bool _auto_size;

		};

	}
}