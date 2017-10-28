#pragma once
#include "utility/UTF8String.h"
#include "gui/Control.h"
#include "gui/StyleManager.h"

namespace hvn3 {
	namespace Gui {

		class ImageableControl : public virtual Control {

		public:
			ImageableControl();

			virtual const ResourceHandle<Drawing::Bitmap> Image();
			virtual void SetImage(BitmapResourceId image);

		private:
			BitmapResourceId _image;

		};

	}
}