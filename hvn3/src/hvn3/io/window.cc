#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/core/engine.h"
#include "hvn3/io/window.h"
#include "hvn3/graphics/Graphics.h"

#include <allegro5/allegro.h>

#include <cassert>
#include <limits>
#include <cmath>

namespace hvn3::io {

	// Public members

	Window::Window(int width, int height) :
		Window(width, height, "") {}
	Window::Window(int width, int height, WindowOptions options) :
		Window(width, height, "", options) {
	}
	Window::Window(int width, int height, const String& title) :
		Window(width, height, title, WindowOptions::None) {}
	Window::Window(int width, int height, const String& title, WindowOptions options) :
		creationSize(width, height) {

		// Core module is required in order to create windows (displays).

		core::Engine::Initialize(core::EngineModules::Core);

		// Set the new window settings.

		SetNewWindowTitle(title);
		SetNewWindowOptions(options);

		// Create the window.

		window = std::shared_ptr<ALLEGRO_DISPLAY>(al_create_display(width, height), FreeDisplay);

		// Convert any bitmaps that may have been created before the window to video bitmaps.

		al_convert_memory_bitmaps();

	}
	Window::Window(const SizeI& size) :
		Window(size.width, size.height) {}
	Window::Window(const SizeI& size, WindowOptions options) :
		Window(size, "", options) {
	}
	Window::Window(const SizeI& size, const String& title) :
		Window(size.width, size.height, title) {}
	Window::Window(const SizeI& size, const String& title, WindowOptions options) :
		Window(size.width, size.height, title, options) {}
	Window::Window(ALLEGRO_DISPLAY* allegroDisplay, bool takeOwnership) {

		if (takeOwnership)
			window = std::shared_ptr<ALLEGRO_DISPLAY>(allegroDisplay, FreeDisplay);
		else
			window = std::shared_ptr<ALLEGRO_DISPLAY>(std::shared_ptr<ALLEGRO_DISPLAY>{}, allegroDisplay);

		creationSize = Size();

	}

	void Window::SetTitle(const String& value) {

		assert(static_cast<bool>(window));

		al_set_window_title(GetAllegroPointer(), value.c_str());

	}

	void Window::SetIcon(const Graphics::Bitmap& icon) {

		assert(static_cast<bool>(window));

		// Make a copy of the icon to prevent it from being freed while we're using it.

		this->icon = icon;

		al_set_display_icon(GetAllegroPointer(), System::AllegroAdapter::ToBitmap(this->icon));

	}

	SizeI Window::Size() const {

		return hvn3::Size<int>(Width(), Height());

	}
	void Window::SetSize(const SizeI& size) {

		SetSize(size.width, size.height);

	}
	void Window::SetSize(int width, int height) {

		assert(static_cast<bool>(window));

		al_resize_display(GetAllegroPointer(), width, height);

	}

	int Window::Width() const {

		assert(static_cast<bool>(window));

		return al_get_display_width(GetAllegroPointer());

	}
	int Window::Height() const {

		assert(static_cast<bool>(window));

		return al_get_display_height(GetAllegroPointer());

	}

	PointI Window::Position() const {

		assert(static_cast<bool>(window));

		PointI result;

		al_get_window_position(GetAllegroPointer(), &result.x, &result.y);

		return result;

	}
	void Window::SetPosition(int x, int y) {

		assert(static_cast<bool>(window));

		al_set_window_position(GetAllegroPointer(), x, y);

	}
	void Window::SetPosition(const PointI& position) {

		SetPosition(position.x, position.y);

	}

	bool Window::IsFullscreen() const {

		return (al_get_display_flags(GetAllegroPointer()) & ALLEGRO_FULLSCREEN_WINDOW) != 0;

	}
	void Window::SetFullscreen(bool value) {

		assert(static_cast<bool>(window));

		if (value != IsFullscreen()) {

			if (value) {

				sizeBeforeFullscreen = Size();
				positionBeforeFullscreen = Position();

			}

			al_set_display_flag(GetAllegroPointer(), ALLEGRO_FULLSCREEN_WINDOW, value);

			if (!value) {

				// Restore the window's prior position before it was made fullscreen.

				SetPosition(positionBeforeFullscreen);

			}

		}

	}

	EventSource Window::GetEventSource() const {

		return class EventSource(al_get_display_event_source(GetAllegroPointer()));

	}
	Graphics::Graphics Window::GetCanvas() {

		return Graphics::Graphics(GetBackBuffer());

	}
	Graphics::Bitmap Window::GetBackBuffer() {

		return Graphics::Bitmap(al_get_backbuffer(GetAllegroPointer()), false);

	}
	ALLEGRO_DISPLAY* Window::GetAllegroPointer() const {

		return window.get();

	}

	void Window::Refresh() {

		al_flip_display();

	}

	// Private members

	void Window::SetNewWindowTitle(const String& title) {

		al_set_new_window_title(title.c_str());

	}
	void Window::SetNewWindowOptions(WindowOptions options) {

		// Apply display flags.

		al_set_new_display_flags(0); // Reset display flags to default

		int displayFlags = 0;

		if (HasFlag(options, WindowOptions::UseOpenGL))
			displayFlags |= ALLEGRO_OPENGL;

		if (HasFlag(options, WindowOptions::UseOpenGL3))
			displayFlags |= ALLEGRO_OPENGL_3_0;

		if (HasFlag(options, WindowOptions::UseForwardCompatibleOpenGL))
			displayFlags |= ALLEGRO_OPENGL_FORWARD_COMPATIBLE;

		if (HasFlag(options, WindowOptions::UseDirect3D))
			displayFlags |= ALLEGRO_DIRECT3D_INTERNAL;

		if (HasFlag(options, WindowOptions::Resizable))
			displayFlags |= ALLEGRO_RESIZABLE;

		if (HasFlag(options, WindowOptions::Borderless))
			displayFlags |= ALLEGRO_FRAMELESS;

		if (HasFlag(options, WindowOptions::FullscreenWindow))
			displayFlags |= ALLEGRO_FULLSCREEN_WINDOW;

		al_set_new_display_flags(displayFlags);

		// Apply display options.

		if (HasFlag(options, WindowOptions::AntiAliased)) {

			al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
			al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

		}

	}
	void Window::FreeDisplay(ALLEGRO_DISPLAY* display) {

		al_destroy_display(display);

		core::Engine::Deinitialize(core::EngineModules::Core);

	}

}