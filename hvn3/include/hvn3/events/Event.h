#pragma once

#include "hvn3/events/EventSource.h"

#include <allegro5/allegro.h>

namespace hvn3 {

	class IUserEvent;

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
		DisplayDisconnected,
		UserEvent = 1024 // Allegro's reserved event types stop at 1024; allows it to be directly casted to ALLEGRO_EVENT_TYPE
	};

	class Event {

		friend class EventQueue;

	public:
		Event();
		~Event();
		
		EventType Type() const;
		EventSource Source() const;
		double Timestamp() const;
		IUserEvent* GetUserEvent();

		ALLEGRO_EVENT* AlPtr();

	private:
		ALLEGRO_EVENT _ev;

		void _beforeUnderlyingEventChanged();
		void _unrefUserEvent();

	};

}