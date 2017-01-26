#include <cassert>
#include <limits>
#include "Display.h"
#include "Graphics.h"
#define UNDEFINED_WINDOW_POSITION_X INT_MAX
#define UNDEFINED_WINDOW_POSITION_Y INT_MAX

Display* Display::__active_display = nullptr;

// Public member functions

Display::Display(int width, int height) :
	Display(width, height, "") {}
Display::Display(int width, int height, const char* title) :
	Display(width, height, title, DisplayFlags::None) {}
Display::Display(int width, int height, const char* title, DisplayFlags flags) :
	Display(UNDEFINED_WINDOW_POSITION_X, UNDEFINED_WINDOW_POSITION_Y, width, height, title, flags) {}
Display::Display(int x, int y, int width, int height) :
	Display(x, y, width, height, "") {}
Display::Display(int x, int y, int width, int height, const char* title) :
	Display(x, y, width, height, title, DisplayFlags::None) {}
Display::Display(int x, int y, int width, int height, const char* title, DisplayFlags flags) :
	ISizeable(width, height),
	__original_size(width, height),
	__size_before_fullscreen(0.0f, 0.0f) {

	// Save the original new display flags so they can be restored.
	int new_display_flags = al_get_new_display_flags();

	// Set new display flags.
	if (x != UNDEFINED_WINDOW_POSITION_X || y != UNDEFINED_WINDOW_POSITION_Y)
		al_set_new_window_position(x, y);
	al_set_new_display_flags(new_display_flags | (int)flags);

	// Create the display.
	__display = al_create_display(width, height);

	//__fullscreen = false;
	SetFocus(true);

	// Restore display flags.
	al_set_new_display_flags(new_display_flags);

	// Assert that the display could be created.
	assert(__display);

}
Display::~Display() {
	if (!__display) return;

	al_destroy_display(__display);
	__display = nullptr;

}

void Display::SetTitle(const char* value) {

	// If the display is null, do nothing.
	if (!__display)
		return;

	// Set the title of the window.
	al_set_window_title(__display, value);

}
void Display::SetIcon(const Bitmap& icon) {

	SetIcon(icon.AlPtr());

}
void Display::SetIcon(const Sprite& icon) {

	SetIcon(icon.AlPtr());

}
void Display::SetIcon(const Sprite* icon) {

	SetIcon(icon->AlPtr());

}
void Display::SetIcon(ALLEGRO_BITMAP* icon) {
	if (!__display) return;

	al_set_display_icon(__display, icon);

}
void Display::Resize(float width, float height) {
	if (!__display) return;

	ISizeable::Resize(width, height);

	// If the Display's actual size is the same as the sizes provided, just update width/height values.
	// This can occur when the Display is resized manually.
	if (al_get_display_width(__display) == width && al_get_display_height(__display) == height)
		return;

	al_resize_display(__display, width, height);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

}
Size Display::Scale() {

	return Size(Width() / __original_size.Width(), Height() / __original_size.Height());

}
Point Display::Position() {

	if (!__display)
		return Point();

	int x, y;
	al_get_window_position(__display, &x, &y);
	return Point(x, y);

}
void Display::SetPosition(int x, int y) {

	if (__display)
		al_set_window_position(__display, x, y);

}
void Display::SetPosition(const Point& position) {

	SetPosition((int)position.X(), (int)position.Y());

}
bool Display::IsFullscreen() const {

	return __fullscreen;

}
void Display::SetFullscreen(bool value) {
	if (!__display) return;

	// If the given value is different than the current state, change display state.
	if (value != __fullscreen) {

		// Set the fullscreen flag.
		__fullscreen = value;

		// If we're going fullscreen, save the size and position of the Display.
		if (__fullscreen) {
			__size_before_fullscreen.Resize(Width(), Height());
			__position_before_fullscreen = Position();
		}

		// Toggle the actual Display flag.
		al_toggle_display_flag(__display, ALLEGRO_FULLSCREEN_WINDOW, __fullscreen);

		if (__fullscreen)
			// If we've gone fullscreen, update the size values to match the new window size.
			ISizeable::Resize(al_get_display_width(__display), al_get_display_height(__display));
		else {
			// If we've left fullscreen, restore the original size and position.
			Resize(__size_before_fullscreen.Width(), __size_before_fullscreen.Height());
			SetPosition(__position_before_fullscreen);
		}

	}

}
bool Display::HasFocus() const {

	return __has_focus;

}
::EventSource Display::EventSource() const {

	return ::EventSource(al_get_display_event_source(__display));

}
Bitmap Display::BackBuffer() const {

	return Bitmap(al_get_backbuffer(__display), false);

}
void Display::Refresh() {

	al_flip_display();

}
ALLEGRO_DISPLAY* Display::AlPtr() const {

	return __display;

}
Display* Display::ActiveDisplay() {

	return __active_display;

}

Display& Display::operator= (Display&& other) {

	__display = other.__display;
	__fullscreen = other.__fullscreen;
	ISizeable::Resize(other.Width(), other.Height());

	other.__display = nullptr;

	return *this;

}

void Display::SetFocus(bool has_focus) {

	__has_focus = has_focus;
	__active_display = this;

}
Size Display::ResolutionToSize(DisplayResolution resolution) {

	switch (resolution) {

	case DisplayResolution::XGA:
		return Size(1024, 768);

	case DisplayResolution::WXGA:
		return Size(1280, 800);

	case DisplayResolution::WXGAPlus:
		return Size(1440, 900);

	case DisplayResolution::FHD:
		return Size(1920, 1080);

	case DisplayResolution::HD:
		return Size(1280, 720);

	case DisplayResolution::VGA:
	default:
		return Size(640, 480);

	}

}

Display::StateAccessor::StateAccessor(Display* display) {

	__display = display;

}
void Display::StateAccessor::SetFocus(bool has_focus) {

	__display->SetFocus(has_focus);

}