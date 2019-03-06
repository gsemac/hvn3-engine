#include "hvn3/gui/WidgetStyle.h"

namespace hvn3 {
	namespace Gui {

		WidgetStyle::BackgroundPositionProperty::BackgroundPositionProperty() {
			flags = PositionFlags::None;
		}


		const Graphics::Bitmap* WidgetStyle::_findBitmap(WidgetProperty p) const {

			auto iter = _bitmaps.find(p);

			if (iter == _bitmaps.end())
				return nullptr;

			return &iter->second;

		}

	}
}