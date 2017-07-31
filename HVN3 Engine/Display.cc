#include <cassert>
#include <limits>
#include <allegro5/allegro.h>
#include "Display.h"
#include "Graphics.h"
#define UNDEFINED_WINDOW_POSITION_X INT_MAX
#define UNDEFINED_WINDOW_POSITION_Y INT_MAX

namespace hvn3 {

	Display* Display::_active_display = nullptr;

	// Public member functions

	Display::Display(int width, int height) :
		Display(width, height, "") {
	}
	Display::Display(int width, int height, const char* title) :
		Display(width, height, title, DisplayFlags::None) {
	}
	Display::Display(int width, int height, const char* title, DisplayFlags flags) :
		Display(UNDEFINED_WINDOW_POSITION_X, UNDEFINED_WINDOW_POSITION_Y, width, height, title, flags) {
	}
	Display::Display(int x, int y, int width, int height) :
		Display(x, y, width, height, "") {
	}
	Display::Display(int x, int y, int width, int height, const char* title) :
		Display(x, y, width, height, title, DisplayFlags::None) {
	}
	Display::Display(int x, int y, int width, int height, const char* title, DisplayFlags flags) :
		SizeableBase(width, height),
		_original_size(width, height),
		_size_before_fullscreen(0.0f, 0.0f) {

		// Save the original new display flags so they can be restored.
		int new_display_flags = al_get_new_display_flags();

		// Set new display flags.
		if (x != UNDEFINED_WINDOW_POSITION_X || y != UNDEFINED_WINDOW_POSITION_Y)
			al_set_new_window_position(x, y);
		al_set_new_display_flags(new_display_flags | (int)flags);
		al_set_new_window_title(title);

		// Create the display.
		_display = al_create_display(width, height);
		_fullscreen = false;
		_has_focus = true;

		// Set this display as the active display.
		_active_display = this;

		// Restore display flags.
		al_set_new_display_flags(new_display_flags);

		// Assert that the display could be created.
		assert(_display);

	}
	Display::~Display() {
		if (!_display) return;

		al_destroy_display(_display);
		_display = nullptr;

	}

	void Display::SetTitle(const char* value) {

		// If the display is null, do nothing.
		if (!_display)
			return;

		// Set the title of the window.
		al_set_window_title(_display, value);

	}
	void Display::SetIcon(const Drawing::Bitmap& icon) {

		SetIcon(icon.AlPtr());

	}
	void Display::SetIcon(const Sprite& icon) {

		SetIcon(icon.SubImage(0).AlPtr());

	}
	void Display::SetIcon(const Sprite* icon) {

		SetIcon(icon->SubImage(0).AlPtr());

	}
	void Display::SetIcon(ALLEGRO_BITMAP* icon) {
		if (!_display) return;

		al_set_display_icon(_display, icon);

	}
	void Display::Resize(int width, int height) {
		
		// If there is not an underlying framework object, do nothing.
		if (_display == nullptr)
			return;

		// Update the size values of the underlying sizeable class.
		SizeableBase::Resize(width, height);

		// If the Display's actual size is the same as the sizes provided, just update width/height values. 
		// This can occur when the Display is resized manually.
		if (al_get_display_width(_display) == width && al_get_display_height(_display) == height)
			return;

		// Update the size of the underlying framework object.
		al_resize_display(_display, width, height);

		// Clear the new regions to black.
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_flip_display();

	}
	Scale Display::Scale() const {

		return hvn3::Scale(static_cast<float>(Width()) / _original_size.Width(), static_cast<float>(Height()) / _original_size.Height());

	}
	PointI Display::Position() const {

		if (!_display)
			return PointI::Empty();

		int x, y;
		al_get_window_position(_display, &x, &y);
		return Point2d<int>(x, y);

	}
	void Display::SetPosition(int x, int y) {

		if (_display)
			al_set_window_position(_display, x, y);

	}
	void Display::SetPosition(const Point2d<int>& position) {

		SetPosition(position.X(), position.Y());

	}
	bool Display::IsFullscreen() const {

		return _fullscreen;

	}
	void Display::SetFullscreen(bool value) {
		if (!_display) return;

		// If the given value is different than the current state, change display state.
		if (value != _fullscreen) {

			// Set the fullscreen flag.
			_fullscreen = value;

			// If we're going fullscreen, save the size and position of the Display.
			if (_fullscreen) {
				_size_before_fullscreen.Resize(Width(), Height());
				_position_before_fullscreen = Position();
			}

			// Toggle the actual Display flag.
			al_toggle_display_flag(_display, ALLEGRO_FULLSCREEN_WINDOW, _fullscreen);

			if (_fullscreen)
				// If we've gone fullscreen, update the size values to match the new window size.
				SizeableBase::Resize(al_get_display_width(_display), al_get_display_height(_display));
			else {
				// If we've left fullscreen, restore the original size and position.
				Resize(_size_before_fullscreen.Width(), _size_before_fullscreen.Height());
				SetPosition(_position_before_fullscreen);
			}

		}

	}
	bool Display::HasFocus() const {

		return _has_focus;

	}
	System::EventSource Display::EventSource() const {

		return System::EventSource(al_get_display_event_source(_display));

	}
	Drawing::Bitmap Display::BackBuffer() const {

		return Drawing::Bitmap(al_get_backbuffer(_display), false);

	}
	void Display::Refresh() {

		al_flip_display();

	}
	ALLEGRO_DISPLAY* Display::AlPtr() const {

		return _display;

	}
	Display* Display::ActiveDisplay() {

		return _active_display;

	}

	Display& Display::operator=(Display&& other) {

		_display = other._display;
		_fullscreen = other._fullscreen;
		SizeableBase::Resize(other.Width(), other.Height());

		other._display = nullptr;

		return *this;

	}

	// Private methods
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

	// Non-class methods
	SizeI ResolutionToSize(DisplayResolution resolution) {

		switch (resolution) {

		case DisplayResolution::XGA:
			return SizeI(1024, 768);

		case DisplayResolution::WXGA:
			return SizeI(1280, 800);

		case DisplayResolution::WXGAPlus:
			return SizeI(1440, 900);

		case DisplayResolution::FHD:
			return SizeI(1920, 1080);

		case DisplayResolution::HD:
			return SizeI(1280, 720);

		case DisplayResolution::VGA:
		default:
			return SizeI(640, 480);

		}

	}

}