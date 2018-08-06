#pragma once
#include "hvn3/graphics/Display.h"
#include "hvn3/events/EventQueue.h"
#include "hvn3/events/Timer.h"
#include "hvn3/core/Properties.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/rooms/RoomManager.h"
#include "hvn3/utility/Stopwatch.h"
#include "hvn3/utility/FpsCounter.h"

namespace hvn3 {

	class DrawEventArgs;
	class Font;
	class IGameManager;
	class IRoom;
	
	namespace System {

		class Runner {

		public:
			Runner(Context context);
			virtual ~Runner();

			// Processes the main game loop.
			virtual void Loop();
			// Terminates the main game loop.
			virtual void Stop();

		protected:
			// Renders the current game state to the display surface.
			virtual void OnDraw(DrawEventArgs& e);
			// Updates the state of the game.
			virtual void OnUpdate(UpdateEventArgs& e);
			// Waits for and processes the next event in the event queue.
			virtual void WaitForEvent();

			// Returns a pointer to the default system font.
			const Font* SystemFont();
			void DrawFps();

			virtual void OnTimerTick(Event& ev);
			virtual void OnDisplayClose(Event& ev);
			virtual void OnKeyDown(Event& ev);
			virtual void OnKeyUp(Event& ev);
			virtual void OnKeyChar(Event& ev);
			virtual void OnMouseButtonDown(Event& ev);
			virtual void OnMouseButtonUp(Event& ev);
			virtual void OnMouseAxes(Event& ev);
			virtual void OnMouseEnterDisplay(Event& ev);
			virtual void OnMouseLeaveDisplay(Event& ev);
			virtual void OnDisplayResize(Event& ev);
			virtual void OnDisplaySwitchOut(Event& ev);
			virtual void OnDisplaySwitchIn(Event& ev);
			virtual void OnRedraw();

		private:
			bool _allow_redraw;
			bool _exit_loop;
			int _frames_skipped;
			EventQueue _event_queue;
			Timer _timer;
			Stopwatch _delta_timer;
			// Flag used to detect when the fullscreen state of the display changes.
			bool _display_was_fullscreen;
			bool _track_mouse_outside_display;
			System::FpsCounter _fps_counter;

			Context _context;
			Font* _default_font;
			Graphics::Graphics _graphics;

			bool _isFrozen();
			void _applyScalingMode();
			void _recalculateMousePosition();
			Properties& _properties();

		};

	}
}