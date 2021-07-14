#pragma once

#include "core/size.h"
#include "core/tagged_handle.h"
#include "core/utf8_string.h"
#include "graphics/bitmap.h"
#include "graphics/graphics.h"
#include "io/display_options.h"
#include "io/idisplay.h"
#include "io/window_options.h"
#include "math/point.h"

#include <memory>

#include <allegro5/allegro.h>

namespace hvn3::io {

	class AllegroDisplay :
		public IDisplay {

	public:
		AllegroDisplay(int width, int height);
		AllegroDisplay(int width, int height, WindowOptions options);
		AllegroDisplay(int width, int height, const String& title);
		AllegroDisplay(int width, int height, const String& title, WindowOptions options);
		AllegroDisplay(const SizeI& size);
		AllegroDisplay(const SizeI& size, WindowOptions options);
		AllegroDisplay(const SizeI& size, const String& title);
		AllegroDisplay(const SizeI& size, const String& title, WindowOptions options);
		AllegroDisplay(const DisplayOptions& options);
		AllegroDisplay(ALLEGRO_DISPLAY* allegroDisplay, bool takeOwnership);

		void SetTitle(const String& value);

		void SetIcon(const graphics::Bitmap& icon);

		SizeI Size() const;
		void SetSize(const SizeI& size);
		void SetSize(int width, int height);

		int Width() const;
		int Height() const;

		math::PointI Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const math::PointI& position);

		bool IsFullscreen() const;
		void SetFullscreen(bool value);

		graphics::Bitmap GetBackBuffer();
		graphics::IGraphics& GetCanvas() override;
		TaggedHandle GetHandle() const;

		uint32_t Id() const override;

		void Refresh() override;
		void Close();

		explicit operator bool() const override;

	private:
		std::shared_ptr<ALLEGRO_DISPLAY> window;
		graphics::Bitmap icon;
		graphics::Graphics canvas;
		SizeI creationSize;
		SizeI sizeBeforeFullscreen;
		math::PointI positionBeforeFullscreen;

		static void SetNewWindowTitle(const String& title);
		static void SetNewWindowOptions(WindowOptions options);
		static void FreeDisplay(ALLEGRO_DISPLAY* display);

	};

}