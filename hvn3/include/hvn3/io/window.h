#pragma once

#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/core/CoreDefs.h"
#include "hvn3/core/SizeableBase.h"
#include "hvn3/events/EventSource.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/graphics/GraphicsDefs.h"
#include "hvn3/graphics/Resolution.h"
#include "hvn3/io/IODefs.h"
#include "hvn3/utility/Size.h"
#include "hvn3/utility/Scale.h"

#include "hvn3/io/window_options.h"
#include "hvn3/utility/Utf8String.h"

#include <memory>
#include <string>

namespace hvn3::io {

	class Window {

	public:
		Window(int width, int height);
		Window(int width, int height, WindowOptions options);
		Window(int width, int height, const String& title);
		Window(int width, int height, const String& title, WindowOptions options);
		Window(const SizeI& size);
		Window(const SizeI& size, WindowOptions options);
		Window(const SizeI& size, const String& title);
		Window(const SizeI& size, const String& title, WindowOptions options);
		Window(ALLEGRO_DISPLAY* allegroDisplay, bool takeOwnership);

		void SetTitle(const String& value);

		void SetIcon(const Graphics::Bitmap& icon);

		SizeI Size() const;
		void SetSize(const SizeI& size);
		void SetSize(int width, int height);

		int Width() const;
		int Height() const;

		PointI Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const PointI& position);

		bool IsFullscreen() const;
		void SetFullscreen(bool value);

		EventSource GetEventSource() const;
		Graphics::Bitmap GetBackBuffer();
		Graphics::Graphics GetCanvas();
		ALLEGRO_DISPLAY* GetAllegroPointer() const;

		void Refresh();

	private:
		std::shared_ptr<ALLEGRO_DISPLAY> window;
		Graphics::Bitmap icon;
		SizeI creationSize;
		SizeI sizeBeforeFullscreen;
		PointI positionBeforeFullscreen;

		static void SetNewWindowTitle(const String& title);
		static void SetNewWindowOptions(WindowOptions options);
		static void FreeDisplay(ALLEGRO_DISPLAY* display);

	};

}