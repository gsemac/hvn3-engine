#include "io/allegro_display.h"

#include "core/engine.h"

namespace hvn3::io {

	// Public members

	AllegroDisplay::AllegroDisplay(int width, int height) :
		AllegroDisplay(width, height, "") {}
	AllegroDisplay::AllegroDisplay(int width, int height, WindowOptions options) :
		AllegroDisplay(width, height, "", options) {
	}
	AllegroDisplay::AllegroDisplay(int width, int height, const String& title) :
		AllegroDisplay(width, height, title, WindowOptions::None) {}
	AllegroDisplay::AllegroDisplay(int width, int height, const String& title, WindowOptions options) :
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
	AllegroDisplay::AllegroDisplay(const SizeI& size) :
		AllegroDisplay(size.width, size.height) {}
	AllegroDisplay::AllegroDisplay(const SizeI& size, WindowOptions options) :
		AllegroDisplay(size, "", options) {
	}
	AllegroDisplay::AllegroDisplay(const SizeI& size, const String& title) :
		AllegroDisplay(size.width, size.height, title) {}
	AllegroDisplay::AllegroDisplay(const SizeI& size, const String& title, WindowOptions options) :
		AllegroDisplay(size.width, size.height, title, options) {}
	AllegroDisplay::AllegroDisplay(const DisplayOptions& options) :
		AllegroDisplay(options.Width(), options.Height(), options.Title(), options.WindowOptions()) {

		// Negative coordinates are used to indicate that the default position should be used.

		if (options.Position().X() >= 0 && options.Position().Y() >= 0)
			SetPosition(options.Position());

	}
	AllegroDisplay::AllegroDisplay(ALLEGRO_DISPLAY* allegroDisplay, bool takeOwnership) {

		if (takeOwnership)
			window = std::shared_ptr<ALLEGRO_DISPLAY>(allegroDisplay, FreeDisplay);
		else
			window = std::shared_ptr<ALLEGRO_DISPLAY>(std::shared_ptr<ALLEGRO_DISPLAY>{}, allegroDisplay);

		creationSize = Size();

	}

	void AllegroDisplay::SetTitle(const String& value) {

		assert(static_cast<bool>(window));

		al_set_window_title(GetHandle(), value.c_str());

	}

	void AllegroDisplay::SetIcon(const graphics::Bitmap& icon) {

		assert(static_cast<bool>(window));

		// Make a copy of the icon to prevent it from being freed while we're using it.

		this->icon = icon;

		al_set_display_icon(GetHandle(), icon.GetUnderlyingData());

	}

	SizeI AllegroDisplay::Size() const {

		return SizeI(Width(), Height());

	}
	void AllegroDisplay::SetSize(const SizeI& size) {

		SetSize(size.width, size.height);

	}
	void AllegroDisplay::SetSize(int width, int height) {

		assert(static_cast<bool>(window));

		al_resize_display(GetHandle(), width, height);

	}

	int AllegroDisplay::Width() const {

		assert(static_cast<bool>(window));

		return al_get_display_width(GetHandle());

	}
	int AllegroDisplay::Height() const {

		assert(static_cast<bool>(window));

		return al_get_display_height(GetHandle());

	}

	math::PointI AllegroDisplay::Position() const {

		assert(static_cast<bool>(window));

		math::PointI result;

		al_get_window_position(GetHandle(), &result.x, &result.y);

		return result;

	}
	void AllegroDisplay::SetPosition(int x, int y) {

		assert(static_cast<bool>(window));

		al_set_window_position(GetHandle(), x, y);

	}
	void AllegroDisplay::SetPosition(const math::PointI& position) {

		SetPosition(position.X(), position.Y());

	}

	bool AllegroDisplay::IsFullscreen() const {

		return (al_get_display_flags(GetHandle()) & ALLEGRO_FULLSCREEN_WINDOW) != 0;

	}
	void AllegroDisplay::SetFullscreen(bool value) {

		assert(static_cast<bool>(window));

		if (value != IsFullscreen()) {

			if (value) {

				sizeBeforeFullscreen = Size();
				positionBeforeFullscreen = Position();

			}

			al_set_display_flag(GetHandle(), ALLEGRO_FULLSCREEN_WINDOW, value);

			if (!value) {

				// Restore the window's prior position before it was made fullscreen.

				SetPosition(positionBeforeFullscreen);

			}

		}

	}

	graphics::IGraphics& AllegroDisplay::GetCanvas() {

		if (!canvas)
			canvas = graphics::Graphics(GetBackBuffer());

		return canvas;

	}
	graphics::Bitmap AllegroDisplay::GetBackBuffer() {

		return graphics::Bitmap(al_get_backbuffer(GetHandle()), false);

	}

	TaggedHandle AllegroDisplay::GetHandle() const {

		return make_tagged_handle(window.get());

	}

	uint32_t AllegroDisplay::Id() const {

		return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(window.get()));

	}

	void AllegroDisplay::Refresh() {

		al_acknowledge_resize(GetHandle());
		al_flip_display();

	}
	void AllegroDisplay::Close() {

		al_destroy_display(GetHandle());

	}

	AllegroDisplay::operator bool() const {

		return static_cast<bool>(GetHandle());

	}

	// Private members

	void AllegroDisplay::SetNewWindowTitle(const String& title) {

		if (String::IsNullOrEmpty(title))
			al_set_new_window_title(title.c_str());

	}
	void AllegroDisplay::SetNewWindowOptions(WindowOptions options) {

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
	void AllegroDisplay::FreeDisplay(ALLEGRO_DISPLAY* display) {

		al_destroy_display(display);

		core::Engine::Deinitialize(core::EngineModules::Core);

	}

}