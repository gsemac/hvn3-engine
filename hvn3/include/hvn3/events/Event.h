#pragma once
#include <allegro5/allegro.h>
#include "hvn3/events/EventSource.h"

namespace hvn3 {
	namespace System {

		enum class EventType {
			Unknown = -1,
			JoystickAxis,
			JoystickButtonDown,
			JoystickButtonUp,
			JoystickConfiguration,
			KeyDown,
			KeyUp,
			KeyChar,
			MouseAxes,
			MouseButtonDown,
			MouseButtonUp,
			MouseWarped,
			MouseEnterDisplay,
			MouseLeaveDisplay,
			TouchBegin,
			TouchEnd,
			TouchMove,
			TouchCancel,
			Timer,
			DisplayExpose,
			DisplayResize,
			DisplayClose,
			DisplayLost,
			DisplayFound,
			DisplaySwitchOut,
			DisplaySwitchIn,
			DisplayOrientation,
			DisplayHaltDrawing,
			DisplayResumeDrawing,
			DisplayConnected,
			DisplayDisconnected
		};

		class Event {
			friend class EventQueue;

		public:
			Event();
			EventType Type() const;
			EventSource Source() const;
			double Timestamp() const;
			ALLEGRO_EVENT* AlPtr();

			// Todo: Make this member private and derive specific event classes.
		public:
			ALLEGRO_EVENT _ev;

		};

	}
}