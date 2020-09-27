#include "core/window.h"

#include "core/engine.h"
#include "events/event_source.h"

namespace hvn3::core {

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

		Engine::Initialize(EngineModules::Core);

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

		al_set_window_title(GetUnderlyingData(), value.c_str());

	}

	void Window::SetIcon(const graphics::Bitmap& icon) {

		assert(static_cast<bool>(window));

		// Make a copy of the icon to prevent it from being freed while we're using it.

		this->icon = icon;

		al_set_display_icon(GetUnderlyingData(), icon.GetUnderlyingData());

	}

	SizeI Window::Size() const {

		return SizeI(Width(), Height());

	}
	void Window::SetSize(const SizeI& size) {

		SetSize(size.width, size.height);

	}
	void Window::SetSize(int width, int height) {

		assert(static_cast<bool>(window));

		al_resize_display(GetUnderlyingData(), width, height);

	}

	int Window::Width() const {

		assert(static_cast<bool>(window));

		return al_get_display_width(GetUnderlyingData());

	}
	int Window::Height() const {

		assert(static_cast<bool>(window));

		return al_get_display_height(GetUnderlyingData());

	}

	math::PointI Window::Position() const {

		assert(static_cast<bool>(window));

		math::PointI result;

		al_get_window_position(GetUnderlyingData(), &result.x, &result.y);

		return result;

	}
	void Window::SetPosition(int x, int y) {

		assert(static_cast<bool>(window));

		al_set_window_position(GetUnderlyingData(), x, y);

	}
	void Window::SetPosition(const math::PointI& position) {

		SetPosition(position.X(), position.Y());

	}

	bool Window::IsFullscreen() const {

		return (al_get_display_flags(GetUnderlyingData()) & ALLEGRO_FULLSCREEN_WINDOW) != 0;

	}
	void Window::SetFullscreen(bool value) {

		assert(static_cast<bool>(window));

		if (value != IsFullscreen()) {

			if (value) {

				sizeBeforeFullscreen = Size();
				positionBeforeFullscreen = Position();

			}

			al_set_display_flag(GetUnderlyingData(), ALLEGRO_FULLSCREEN_WINDOW, value);

			if (!value) {

				// Restore the window's prior position before it was made fullscreen.

				SetPosition(positionBeforeFullscreen);

			}

		}

	}

	const events::IEventSource& Window::GetEventSource() const {

		if (!eventSource)
			eventSource = events::EventSource(al_get_display_event_source(GetUnderlyingData()));

		return eventSource;

	}
	graphics::IGraphics& Window::GetCanvas() {

		if (!canvas)
			canvas = graphics::Graphics(GetBackBuffer());

		return canvas;

	}
	graphics::Bitmap Window::GetBackBuffer() {

		return graphics::Bitmap(al_get_backbuffer(GetUnderlyingData()), false);

	}
	ALLEGRO_DISPLAY* Window::GetUnderlyingData() const {

		return window.get();

	}

	void Window::Refresh() {

		al_flip_display();

	}
	void Window::Close() {

		al_destroy_display(GetUnderlyingData());

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

		Engine::Deinitialize(EngineModules::Core);

	}

}