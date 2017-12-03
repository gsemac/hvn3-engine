#include "hvn3/gui/ImageableControl.h"
#include "hvn3/gui/GuiManager.h"

namespace hvn3 {
	namespace Gui {

		ImageableControl::ImageableControl() {

			_image = BitmapResourceId::Null;

		}

		const AssetHandle<Graphics::Bitmap> ImageableControl::Image() {

			if (Manager() && Manager()->StyleManager())
				return Manager()->StyleManager()->GetImageResource(_image);

			return nullptr;

		}
		void ImageableControl::SetImage(BitmapResourceId image) {

			_image = image;

			Invalidate();

		}

	}
}