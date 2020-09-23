#pragma once
#include <string>

namespace hvn3 {
	namespace Imaging {

		enum class ImageFormat {
			Bmp,
			Jpeg,
			Png,
			Tiff
		};

		std::string ImageFormatToFileExtension(ImageFormat format);
		ImageFormat FileExtensionToImageFormat(const std::string& extension);

	}
}