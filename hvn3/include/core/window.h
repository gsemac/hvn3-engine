#pragma once

#include "core/utf8_string.h"
#include "core/window_options.h"
#include "events/read_only_event_source.h"
#include "graphics/bitmap.h"
#include "graphics/graphics.h"
#include "math/point.h"
#include "utilities/size.h"

#include <memory>

#include <allegro5/allegro.h>

namespace hvn3::core {

	class Window {

	public:
		Window(int width, int height);
		Window(int width, int height, WindowOptions options);
		Window(int width, int height, const core::String& title);
		Window(int width, int height, const core::String& title, WindowOptions options);
		Window(const utilities::SizeI& size);
		Window(const utilities::SizeI& size, WindowOptions options);
		Window(const utilities::SizeI& size, const core::String& title);
		Window(const utilities::SizeI& size, const core::String& title, WindowOptions options);
		Window(ALLEGRO_DISPLAY* allegroDisplay, bool takeOwnership);

		void SetTitle(const core::String& value);

		void SetIcon(const graphics::Bitmap& icon);

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
		graphics::Bitmap GetBackBuffer();
		graphics::IGraphics& GetCanvas();
		ALLEGRO_DISPLAY* GetUnderlyingData() const;

		void Refresh();
		void Close();

	private:
		std::shared_ptr<ALLEGRO_DISPLAY> window;
		mutable events::ReadOnlyEventSource eventSource;
		graphics::Bitmap icon;
		graphics::Graphics canvas;
		utilities::SizeI creationSize;
		utilities::SizeI sizeBeforeFullscreen;
		math::PointI positionBeforeFullscreen;

		static void SetNewWindowTitle(const core::String& title);
		static void SetNewWindowOptions(WindowOptions options);
		static void FreeDisplay(ALLEGRO_DISPLAY* display);

	};

}