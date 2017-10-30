#include "graphics/Imaging.h"
#include <algorithm>

namespace hvn3 {
	namespace Imaging {

		std::string ImageFormatToFileExtension(ImageFormat format) {

			switch (format) {
			case ImageFormat::Jpeg:
				return ".jpeg";
			case ImageFormat::Png:
				return ".png";
			case ImageFormat::Tiff:
				return ".tiff";
			default:
			case ImageFormat::Bmp:
				return ".bmp";
			}

		}
		ImageFormat FileExtensionToImageFormat(const std::string& extension) {

			std::string ext = extension;
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

			if (extension == ".png")
				return ImageFormat::Png;

			if (extension == ".jpg" || extension == ".jpeg")
				return ImageFormat::Jpeg;

			if (extension == ".tif" || extension == ".tiff")
				return ImageFormat::Tiff;

			return ImageFormat::Bmp;

		}

	}
}