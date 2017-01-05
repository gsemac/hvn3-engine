#pragma once
#include <allegro5\allegro.h>
#include <string>
#include "ISizeable.h"
#include "Size.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "BitFlags.h"
#include "EventSource.h"

enum class DisplayResolution {
	XGA,
	WXGA,
	WXGAPlus,
	VGA,
	HD,
	FHD,
	Fullscreen
};

enum class DisplayOptions {
	None = 0x00,
	Resizable = 0x10,
	NoBorder = 0x20
};
ENABLE_BITFLAG_OPERATORS(DisplayOptions);

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

	Display();
	Display(DisplayResolution resolution, DisplayOptions flags = DisplayOptions::None);
	Display(float width, float height, DisplayOptions flags = DisplayOptions::None);
	Display(Size size, DisplayOptions flags = DisplayOptions::None);
	~Display();

	void SetTitle(const char* value);
	void SetTitle(std::string value);

	void SetIcon(const Bitmap& icon);
	void SetIcon(const Sprite& icon);
	void SetIcon(const Sprite* icon);
	void SetIcon(ALLEGRO_BITMAP* icon);

	void Resize(float width, float height) override;

	bool IsFullscreen() const;
	void SetFullscreen(bool value);
	bool HasFocus() const;
	::EventSource EventSource() const;
	Bitmap BackBuffer() const;
	ALLEGRO_DISPLAY* AlPtr() const;
	void Flip();

	static Display* ActiveDisplay();

	Display& Display::operator= (Display&& other);
	
private:
	ALLEGRO_DISPLAY* __display;
	bool __fullscreen;
	bool __has_focus;
	static Display* __active_display;

	void SetFocus(bool has_focus);
	Size ResolutionToSize(DisplayResolution resolution);

};