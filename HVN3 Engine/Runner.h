#pragma once
#include "Display.h"
#include "EventQueue.h"
#include "Timer.h"
#include "Properties.h"
#include "Graphics.h"
#include "RoomManager.h"
#include "Stopwatch.h"

namespace hvn3 {

	class Font;
	class IRoom;
	class UpdateEventArgs;
	class DrawEventArgs;

	class Runner {

	public:
		Runner(Properties& properties, RoomManager& room_manager);
		~Runner();

		// Processes the main game loop.
		virtual void Loop();

		// Returns an object representing a collection of game properties. 
		const Properties& Properties() const;

		IRoom* CurrentRoom();

	protected:
		// Renders the current game state to the display surface.
		virtual void Draw(DrawEventArgs& e);
		// Updates the state of the game.
		virtual void Update(UpdateEventArgs& e);
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

		RoomManager& _room_manager;
		Display _display;
		Font* _default_font;
		hvn3::Properties& _properties;
		Drawing::Graphics _graphics;

		void ApplyScalingMode();
		void RecalculateMousePosition();

	};

}