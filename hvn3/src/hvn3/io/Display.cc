#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/core/Engine.h"
#include "hvn3/io/Display.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/io/DisplayListener.h"

#include <allegro5/allegro.h>

#include <cassert>
#include <limits>
#include <cmath>

#define UNDEFINED_WINDOW_POSITION_X INT_MAX
#define UNDEFINED_WINDOW_POSITION_Y INT_MAX

namespace hvn3 {

	Display* Display::_active_display = nullptr;

	// Public member functions

	Display::Display(int width, int height) :
		Display(width, height, "") {
	}
	Display::Display(int width, int height, const std::string& title) :
		Display(width, height, title, static_cast<DisplayFlags>(0)) {
	}
	Display::Display(int width, int height, const std::string& title, DisplayFlags flags) :
		Display(UNDEFINED_WINDOW_POSITION_X, UNDEFINED_WINDOW_POSITION_Y, width, height, title, flags) {
	}
	Display::Display(const SizeI& size) :
		Display(size.Width(), size.Height()) {
	}
	Display::Display(const SizeI& size, const std::string& title) :
		Display::Display(size.width, size.height, title) {
	}
	Display::Display(const SizeI& size, const std::string& title, DisplayFlags flags) :
		Display(size.Width(), size.Height(), title, flags) {
	}
	Display::Display(int x, int y, int width, int height) :
		Display(x, y, width, height, "") {
	}
	Display::Display(int x, int y, int width, int height, const std::string& title) :
		Display(x, y, width, height, title, static_cast<DisplayFlags>(0)) {
	}
	Display::Display(int x, int y, int width, int height, const std::string& title, DisplayFlags flags) :
		_original_size(width, height),
		_size_before_fullscreen(0, 0) {

		// Make sure that the framework has been initialized before attempting to create a display.
		Engine::RequireComponent(EngineComponent::Core);

		// Set the position at which new displays are created.
		if (x != UNDEFINED_WINDOW_POSITION_X || y != UNDEFINED_WINDOW_POSITION_Y)
			al_set_new_window_position(x, y);

		// The set the title with which new displays are created.
		al_set_new_window_title(title.c_str());

		// Store the default display flags before altering them, so they can be restored after creating the display.
		int new_display_flags = al_get_new_display_flags();
		al_set_new_display_flags(new_display_flags | System::AllegroAdapter::ToDisplayFlags(flags));

		// Set the appropriate flags to enable anti-aliasing, if applicable.
		if (HasFlag(flags, DisplayFlags::AntiAlias)) {
			al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
			al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
		}

		// Create the underlying Allegro object representing the display.
		_display = std::shared_ptr<ALLEGRO_DISPLAY>(al_create_display(width, height), al_destroy_display);

		// Restore the new display flags to the default flags.
		al_set_new_display_flags(new_display_flags);

		// If any bitmaps were created before this display was created, tie them to the display.
		Graphics::Bitmap::ConvertMemoryBitmapsToVideoBitmaps();

		// Initialize remaining members.
		_fullscreen = false;
		_has_focus = true;
		_active_display = this;
		_owns_display = true;

		if (_display)
			_back_buffer = Graphics::Bitmap(al_get_backbuffer(_display.get()), false);

	}
	Display::Display(Graphics::Resolution resolution) :
		Display(Graphics::ResolutionToSize(resolution)) {
	}
	Display::Display(ALLEGRO_DISPLAY* allegroPointer) :
		_original_size(al_get_display_width(allegroPointer), al_get_display_height(allegroPointer)),
		_size_before_fullscreen(0, 0) {

		assert(allegroPointer != nullptr);

		// The class will act as an interface around the pointer, and will be non-owning and will not free the display upon destruction.

		_display = std::shared_ptr<ALLEGRO_DISPLAY>(std::shared_ptr<ALLEGRO_DISPLAY>{}, allegroPointer);
		_fullscreen = static_cast<bool>(al_get_display_flags(allegroPointer) & ALLEGRO_FULLSCREEN_WINDOW);
		_has_focus = false;
		_back_buffer = Graphics::Bitmap(al_get_backbuffer(allegroPointer), false);
		_owns_display = false;

	}
	Display::~Display() {

		if (_display && _owns_display) {

			if (ActiveDisplay() == this)
				_active_display = nullptr;

			// Keep in mind that we might have a display constructed with the default move constructor. When that happens, we can end up releasing the component twice.
			// To avoid that, we check here that we're actually deinitializing a display first.
			Engine::ReleaseComponent(EngineComponent::Core);

		}

	}

	void Display::SetTitle(const std::string& value) {

		if (!_display)
			return;

		al_set_window_title(get(), value.c_str());

	}
	void Display::SetIcon(const Graphics::Bitmap& icon) {

		_icon = icon;

		if (!_display)
			return;

		al_set_display_icon(get(), System::AllegroAdapter::ToBitmap(_icon));

	}
	void Display::Resize(const hvn3::Size<int>& newSize) {
		Resize(newSize.width, newSize.height);
	}
	void Display::Resize(int width, int height) {

		assert(static_cast<bool>(_display));

		// Update the size of the display.
		al_resize_display(get(), width, height);

	}
	hvn3::Size<int> Display::Size() const {
		return hvn3::Size<int>(Width(), Height());
	}
	int Display::Width() const {

		if (_display)
			return al_get_display_width(_display.get());

		return 0;

	}
	int Display::Height() const {

		if (_display)
			return al_get_display_height(_display.get());

		return 0;

	}
	PointI Display::Position() const {

		if (!_display)
			return PointI::Empty();

		PointI out;
		al_get_window_position(get(), &out.x, &out.y);

		return out;

	}
	void Display::SetPosition(int x, int y) {

		if (_display)
			al_set_window_position(get(), x, y);

	}
	void Display::SetPosition(const PointI& position) {

		SetPosition(position.X(), position.Y());

	}
	bool Display::IsFullscreen() const {

		return _fullscreen;

	}
	void Display::SetFullscreen(bool value) {

		if (!_display)
			return;

		// If the given value is different than the current state, change display state.
		if (value != _fullscreen) {

			// Set the fullscreen flag.
			_fullscreen = value;

			// If we're going fullscreen, save the size and position of the Display.
			if (_fullscreen) {
				_size_before_fullscreen.SetSize(Width(), Height());
				_position_before_fullscreen = Position();
			}

			// Toggle the actual Display flag.
			al_set_display_flag(get(), ALLEGRO_FULLSCREEN_WINDOW, _fullscreen);

			if (!_fullscreen) {

				// If we've left fullscreen, restore the original position.
				SetPosition(_position_before_fullscreen);

			}

		}

	}
	bool Display::IsFocused() const {
		return _has_focus;
	}
	void Display::SetCursorVisible(bool value) {

		if (value)
			al_show_mouse_cursor(get());
		else
			al_hide_mouse_cursor(get());

	}
	void Display::SetCursor(SystemCursor cursor) {
		al_set_system_mouse_cursor(get(), (ALLEGRO_SYSTEM_MOUSE_CURSOR)cursor);
	}
	EventSource Display::EventSource() const {
		return class EventSource(al_get_display_event_source(get()));
	}
	Graphics::Bitmap& Display::GetBackBuffer() {
		return _back_buffer;
	}
	Graphics::Graphics Display::Canvas() {
		return Graphics::Graphics(GetBackBuffer());
	}
	void Display::Refresh() {
		al_flip_display();
	}
	ALLEGRO_DISPLAY* Display::get() const {
		return _display.get();
	}

	Display* Display::ActiveDisplay() {
		return _active_display;
	}
	int Display::GetDefaultDisplayOption(DisplayOption option) {

		int importance;

		return al_get_new_display_option(System::AllegroAdapter::ToDisplayOption(option), &importance);

	}
	void Display::SetDefaultDisplayOption(DisplayOption option, int value) {

		al_set_new_display_option(System::AllegroAdapter::ToDisplayOption(option), value, ALLEGRO_SUGGEST);

	}



	void Display::_setFocus(bool value) {

		_has_focus = value;

		if (value) {

			// If the display is now focused, make it the active display.
			Display::_active_display = this;

		}
		else if (Display::_active_display == this) {

			// If the display has lost focus and was previously the active display, set the active display to null.
			Display::_active_display = nullptr;

		}

	}
	Scale Display::_scale() const {

		float w = static_cast<float>(Width());
		float h = static_cast<float>(Height());
		float ow = static_cast<float>(_original_size.Width());
		float oh = static_cast<float>(_original_size.Height());

		return hvn3::Scale(w / ow, h / oh);

	}

}