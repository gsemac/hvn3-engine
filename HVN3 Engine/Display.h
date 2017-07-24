#pragma once
#include <string>
#include "SizeableBase.h"
#include "Size.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "BitFlags.h"
#include "EventSource.h"
#include "Scale.h"

struct ALLEGRO_DISPLAY;

namespace hvn3 {

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
		Resizable = 16, // ALLEGRO_RESIZABLE
		NoBorder = 32, // ALLEGRO_NOFRAME
		OpenGL = 4, // ALLEGRO_OPENGL
		OpenGL3 = 128, // ALLEGRO_OPENGL_3_0
		OpenGLForwardCompatible = 256, // ALLEGRO_OPENGL_FORWARD_COMPATIBLE
		Direct3D = 8
	};
	ENABLE_BITFLAG_OPERATORS(DisplayFlags);

	namespace System {
		class DisplayController;
	}

	class Display : public SizeableBase<int> {
		friend class System::DisplayController;

	public:
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

		void Resize(int width, int height) override;
		// Returns the current scale factor relative to the size at which the display was initialized.
		Scale Scale() const;
		PointI Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const Point2d<int>& position);

		bool IsFullscreen() const;
		void SetFullscreen(bool value);
		bool HasFocus() const;
		System::EventSource EventSource() const;
		Drawing::Bitmap BackBuffer() const;
		ALLEGRO_DISPLAY* AlPtr() const;
		void Refresh();

		static Display* ActiveDisplay();

		Display& Display::operator=(Display&& other);

	private:
		// Pointer to the underlying framework object.
		ALLEGRO_DISPLAY* _display;
		// Whether or not the display is the active window.
		bool _has_focus;
		// Whether or not the display is currently full-screen.
		bool _fullscreen;
		// The size that the display was created with. Used to calculate scale factor when the display is resized.
		SizeI _original_size;
		// The size of the window prior to being full-screen. Used to restore original size.
		SizeI _size_before_fullscreen;
		// The position of the window prior to being full-screen. Used to restore original position.
		PointI _position_before_fullscreen;

		// Pointer to the currently-active display.
		static Display* _active_display;

		// Sets a flag indicating whether or not the display currently has focus.
		void _SetFocus(bool value);

	};

	// Returns a set of dimensions according to the given resolution.
	SizeI ResolutionToSize(DisplayResolution resolution);

}