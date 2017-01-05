#include <cassert>
#include "Display.h"
#include "Graphics.h"

Display* Display::__active_display = nullptr;

// Public member functions

Display::Display() {

	__display = nullptr;
	__fullscreen = false;
	__has_focus = false;

}
Display::Display(DisplayResolution resolution, DisplayOptions flags) :
	Display(ResolutionToSize(resolution).Width(), ResolutionToSize(resolution).Height(), flags) {

	if (resolution == DisplayResolution::Fullscreen)
		SetFullscreen(true);

}
Display::Display(float width, float height, DisplayOptions flags) : ISizeable(width, height) {

	// Save the original new display flags so they can be restored.
	int new_display_flags = al_get_new_display_flags();

	// Set new display flags.
	al_set_new_display_flags(new_display_flags | (int)flags);

	// Create the display.
	__display = al_create_display(width, height);
	__fullscreen = false;
	SetFocus(true);

	// Restore display flags.
	al_set_new_display_flags(new_display_flags);

	// Assert that the display could be created.
	assert(__display);

	// Set the drawing target to the back buffer of the display.
	Graphics::SetDrawingTarget(BackBuffer());

}
Display::Display(Size size, DisplayOptions flags) : Display(size.Width(), size.Height(), flags) {}
Display::~Display() {
	if (!__display) return;

	//al_destroy_display(__display);
	__display = nullptr;

}
Display& Display::operator= (Display&& other) {

	__display = other.__display;
	__fullscreen = other.__fullscreen;
	ISizeable::Resize(other.Width(), other.Height());

	other.__display = nullptr;

	return *this;

}

void Display::SetTitle(const char* value) {
	if (!__display) return;

	al_set_window_title(__display, value);

}
void Display::SetTitle(std::string value) {
	if (!__display) return;

	SetTitle(value.c_str());

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
bool Display::IsFullscreen() const {

	return __fullscreen;

}
void Display::SetFullscreen(bool value) {
	if (!__display) return;

	// If the given value is different than the current state, change display state.
	if ((value && !__fullscreen) || (!value && __fullscreen)) {

		// Change display mode.
		__fullscreen = value;
		al_toggle_display_flag(__display, ALLEGRO_FULLSCREEN_WINDOW, __fullscreen);
		ISizeable::Resize(al_get_display_width(__display), al_get_display_height(__display));

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
void Display::Flip() {

	al_flip_display();

}
ALLEGRO_DISPLAY* Display::AlPtr() const {

	return __display;

}
Display* Display::ActiveDisplay() {

	return __active_display;

}

Display::StateAccessor::StateAccessor(Display* display) {

	__display = display;

}
void Display::StateAccessor::SetFocus(bool has_focus) {

	__display->SetFocus(has_focus);

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