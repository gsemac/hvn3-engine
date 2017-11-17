#include "gui/ImageableControl.h"
#include "gui/GuiManager.h"

namespace hvn3 {
	namespace Gui {

		ImageableControl::ImageableControl() {

			_image = BitmapResourceId::Null;

		}

		const ResourceHandle<Graphics::Bitmap> ImageableControl::Image() {

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