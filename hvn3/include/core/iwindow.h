#pragma once

#include "math/point.h"
#include "utilities/size.h"

namespace hvn3::core {

	class IWindow {

	public:
		utilities::SizeI Size() const;
		void SetSize(const utilities::SizeI& size);
		void SetSize(int width, int height);

		int Width() const;
		int Height() const;

		math::PointI Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const math::PointI& position);

		bool IsFullscreen() const;
		void SetFullscreen(bool value);

		const events::IEventSource& GetEventSource() const;
		graphics::IGraphics& GetCanvas();

		void Refresh();
		void Close();

	};

}