#pragma once
#include "hvn3/utility/UTF8String.h"
#include "hvn3/gui/Control.h"
#include "hvn3/gui/StyleManager.h"

namespace hvn3 {
	namespace Gui {

		class ImageableControl : public virtual Control {

		public:
			ImageableControl();

			virtual const AssetHandle<Graphics::Bitmap> Image();
			virtual void SetImage(BitmapResourceId image);

		private:
			BitmapResourceId _image;

		};

	}
}