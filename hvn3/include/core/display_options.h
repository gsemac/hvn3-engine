#pragma once

#include "core/size.h"
#include "core/utf8_string.h"
#include "core/window_options.h"
#include "math/point.h"

namespace hvn3::core {

	class DisplayOptions {

	public:
		int width;
		int height;
		int x;
		int y;
		String title;
		WindowOptions windowOptions;

		DisplayOptions();

		int Width() const;
		void SetWidth(int width);
		int Height() const;
		void SetHeight(int height);
		SizeI Size() const;
		void SetSize(int width, int height);
		void SetSize(const SizeI& size);
		int X() const;
		void SetX(int x);
		int Y() const;
		void SetY(int y);
		math::PointI Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const math::PointI& position);
		const String& Title() const;
		void SetTitle(const String& title);
		WindowOptions WindowOptions() const;
		void SetWindowOptions(enum WindowOptions windowOptions);

	};

}