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

		private:
			Button();

			void _initializeMembers();

			bool _auto_size;

		};

	}
}