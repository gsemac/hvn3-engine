#pragma once

namespace hvn3::graphics {

	enum class PixelFormat {
		Any,
		AnyNoAlpha,
		AnyWithAlpha,
		Any15NoAlpha,
		Any15WithAlpha,
		Any16NoAlpha,
		Any16WithAlpha,
		Any24NoAlpha,
		Any24WithAlpha,
		Any32NoAlpha,
		Any32WithAlpha,
		Argb8888,
		Rgba8888,
		Argb4444,
		Rgb888,
		Rgb565,
		Rgb555,
		Rgba5551,
		Argb1555,
		Abgr8888,
		Xbgr8888,
		Bgr888,
		Bgr565,
		Bgr555,
		Rgbx8888,
		Xrgb8888,
		AbgrF32,
		Abgr8888LE
	};

}