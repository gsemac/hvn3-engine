#pragma once
#include "hvn3/gui/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Button : public WidgetBase {

		public:
			Button(const String& text);
			Button(float x, float y, const String& text);
			Button(const PointF& position, const String& text);
			Button(float x, float y, float width, float height, const String& text);
			Button(const PointF& position, const SizeF& size, const String& text);

			void OnRendererChanged(WidgetRendererChangedEventArgs& e) override;

		private:
			void _initializeMembers();

			bool _auto_size;

		};

	}
}