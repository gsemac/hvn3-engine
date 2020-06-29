#include "graphics/image_format.h"

namespace hvn3::graphics {

	std::string GetImageFormatFileExtension(ImageFormat imageFormat) {

		switch (imageFormat) {

		case ImageFormat::Jpeg:
			return ".jpeg";

		case ImageFormat::Png:
			return ".png";

		case ImageFormat::Tiff:
			return ".tiff";

		default:
			return ".bmp";

		}

	}

}