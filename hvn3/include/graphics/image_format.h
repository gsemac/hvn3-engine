#pragma once

#include <string>

namespace hvn3::graphics {

	enum class ImageFormat {
		Bmp,
		Jpeg,
		Png,
		Tiff
	};

	std::string GetImageFormatFileExtension(ImageFormat imageFormat);

}