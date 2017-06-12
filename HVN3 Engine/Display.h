#pragma once
#include <string>
#include "ISizeable.h"
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

	class Display : public ISizeable<int> {
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

		void Resize(int width, int height) override;
		// Returns the current scale factor relative to the size at which the display was initialized.
		Scale Scale() const;
		Point2d<int> Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const Point2d<int>& position);

		bool IsFullscreen() const;
		void SetFullscreen(bool value);
		bool HasFocus() const;
		hvn3::EventSource EventSource() const;
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
		hvn3::Size<int> __original_size;
		hvn3::Size<int> __size_before_fullscreen;
		Point2d<int> __position_before_fullscreen;

		void SetFocus(bool has_focus);
		hvn3::Size<int> ResolutionToSize(DisplayResolution resolution);

	};

}