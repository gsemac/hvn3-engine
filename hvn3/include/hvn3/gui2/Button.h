#pragma once
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/utility/Utf8String.h"

namespace hvn3 {
	namespace Gui {

		class Button : public WidgetBase {

		public:
			Button(float x, float y, const String& text);
			Button(const hvn3::PointF& position, const String& text);
			Button(float x, float y, float width, float height, const String& text);
			Button(const hvn3::PointF& position, const SizeF& size, const const String& text);
			
		private:
			Button();

			void _initializeMembers();

			String _text;
			bool _auto_size;

		};

	}
}