#pragma once

#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/core/CoreDefs.h"
#include "hvn3/core/SizeableBase.h"
#include "hvn3/events/EventSource.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/graphics/GraphicsDefs.h"
#include "hvn3/graphics/Resolution.h"
#include "hvn3/io/IODefs.h"
#include "hvn3/utility/Size.h"
#include "hvn3/utility/Scale.h"

#include <memory>
#include <string>

namespace hvn3 {

	namespace System {
		class Runner;
	}

	class Display {
		friend class System::Runner;

	public:
		Display(int width, int height);
		Display(int width, int height, const std::string& title);
		Display(int width, int height, const std::string& title, DisplayFlags flags);
		Display(const SizeI& size);
		Display(const SizeI& size, const std::string& title);
		Display(const SizeI& size, const std::string& title, DisplayFlags flags);
		Display(int x, int y, int width, int height);
		Display(int x, int y, int width, int height, const std::string& title);
		Display(int x, int y, int width, int height, const std::string& title, DisplayFlags flags);
		Display(Graphics::Resolution resolution);
		Display(ALLEGRO_DISPLAY* allegroPointer);
		~Display();

		void SetTitle(const std::string& value);
		void SetIcon(const Graphics::Bitmap& icon);

		void Resize(const hvn3::Size<int>& newSize);
		void Resize(int width, int height);
		hvn3::Size<int> Size() const;
		int Width() const;
		int Height() const;
		PointI Position() const;
		void SetPosition(int x, int y);
		void SetPosition(const PointI& position);
		bool IsFullscreen() const;
		void SetFullscreen(bool value);
		bool IsFocused() const;
		void SetCursorVisible(bool value);
		void SetCursor(SystemCursor cursor);

		Graphics::Bitmap& GetBackBuffer();
		Graphics::Graphics Canvas();

		void Refresh();
		EventSource EventSource() const;
		ALLEGRO_DISPLAY* get() const;

		static Display* ActiveDisplay();
		static int GetDefaultDisplayOption(DisplayOption option);
		static void SetDefaultDisplayOption(DisplayOption option, int value);

	private:
		std::shared_ptr<ALLEGRO_DISPLAY> _display; // Pointer to the underlying Allegro object.
		bool _owns_display;
		Graphics::Bitmap _back_buffer; // Non-owning bitmap representing the back-buffer
		Graphics::Bitmap _icon; // The icon associated with the window
		bool _has_focus; // Whether or not the display is the active window.
		bool _fullscreen; // Whether or not the display is currently full-screen.
		SizeI _original_size; // The size that the display was created with. Used to calculate scale factor when the display is resized.
		SizeI _size_before_fullscreen; // The size of the window prior to being full-screen. Used to restore original size.		
		PointI _position_before_fullscreen; // The position of the window prior to being full-screen. Used to restore original position.		

		static Display* _active_display; // Pointer to the currently-active display.

		// Sets a flag indicating whether or not the display currently has focus.
		void _setFocus(bool value);
		Scale _scale() const;

	};

}