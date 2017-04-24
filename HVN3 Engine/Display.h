#pragma once
#include <allegro5\allegro.h>
#include <string>
#include "ISizeable.h"
#include "Size.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "BitFlags.h"
#include "EventSource.h"
#include "Scale.h"

enum class DisplayResolution {
	XGA,
	WXGA,
	WXGAPlus,
	VGA,
	HD,
	FHD,
	Fullscreen
};

enum class DisplayFlags {
	None = 0,
	Resizable = ALLEGRO_RESIZABLE,
	NoBorder = ALLEGRO_NOFRAME,
	OpenGL = ALLEGRO_OPENGL,
	OpenGL3 = ALLEGRO_OPENGL_3_0,
	OpenGLForwardCompatible = ALLEGRO_OPENGL_FORWARD_COMPATIBLE,
	Direct3D = 8
};
ENABLE_BITFLAG_OPERATORS(DisplayFlags);

class Display : public ISizeable {
	friend class StateAccessor;

public:
	class StateAccessor {

	public:
		StateAccessor(Display* display);
		void SetFocus(bool has_focus);

	private:
		Display* __display;

	};

	Display(int width, int height);
	Display(int width, int height, const char* title);
	Display(int width, int height, const char* title, DisplayFlags flags);
	Display(int x, int y, int width, int height);
	Display(int x, int y, int width, int height, const char* title);
	Display(int x, int y, int width, int height, const char* title, DisplayFlags flags);
	~Display();

	void SetTitle(const char* value);
	void SetIcon(const Drawing::Bitmap& icon);
	void SetIcon(const Sprite& icon);
	void SetIcon(const Sprite* icon);
	void SetIcon(ALLEGRO_BITMAP* icon);

	void Resize(float width, float height) override;
	// Returns the current scale factor relative to the size at which the display was initialized.
	Scale Scale() const;
	Point Position() const;
	void SetPosition(int x, int y);
	void SetPosition(const Point& position);

	bool IsFullscreen() const;
	void SetFullscreen(bool value);
	bool HasFocus() const;
	::EventSource EventSource() const;
	Drawing::Bitmap BackBuffer() const;
	ALLEGRO_DISPLAY* AlPtr() const;
	void Refresh();

	static Display* ActiveDisplay();

	Display& Display::operator= (Display&& other);

private:
	ALLEGRO_DISPLAY* __display;
	bool __fullscreen;
	bool __has_focus;
	static Display* __active_display;
	Size __original_size;
	Size __size_before_fullscreen;
	Point __position_before_fullscreen;

	void SetFocus(bool has_focus);
	Size ResolutionToSize(DisplayResolution resolution);

};