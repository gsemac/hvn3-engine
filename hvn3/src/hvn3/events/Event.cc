#include "hvn3/events/Event.h"

namespace hvn3 {

	Event::Event() {}
	EventType Event::Type() const {

		switch (_ev.type) {
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			return EventType::JoystickAxis;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			return EventType::JoystickButtonDown;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
			return EventType::JoystickButtonUp;
		case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
			return EventType::JoystickConfiguration;
		case ALLEGRO_EVENT_KEY_DOWN:
			return EventType::KeyDown;
		case ALLEGRO_EVENT_KEY_CHAR:
			return EventType::KeyChar;
		case ALLEGRO_EVENT_KEY_UP:
			return EventType::KeyUp;
		case ALLEGRO_EVENT_MOUSE_AXES:
			return EventType::MouseAxes;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			return EventType::MouseButtonDown;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			return EventType::MouseButtonUp;
		case ALLEGRO_EVENT_MOUSE_WARPED:
			return EventType::MouseWarped;
		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
			return EventType::MouseEnterDisplay;
		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			return EventType::MouseLeaveDisplay;
		case ALLEGRO_EVENT_TOUCH_BEGIN:
			return EventType::TouchBegin;
		case ALLEGRO_EVENT_TOUCH_END:
			return EventType::TouchEnd;
		case ALLEGRO_EVENT_TOUCH_MOVE:
			return EventType::TouchMove;
		case ALLEGRO_EVENT_TOUCH_CANCEL:
			return EventType::TouchCancel;
		case ALLEGRO_EVENT_TIMER:
			return EventType::Timer;
		case ALLEGRO_EVENT_DISPLAY_EXPOSE:
			return EventType::DisplayExpose;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			return EventType::DisplayResize;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			return EventType::DisplayClose;
		case ALLEGRO_EVENT_DISPLAY_LOST:
			return EventType::DisplayLost;
		case ALLEGRO_EVENT_DISPLAY_FOUND:
			return EventType::DisplayFound;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
			return EventType::DisplaySwitchOut;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
			return EventType::DisplaySwitchIn;
		case ALLEGRO_EVENT_DISPLAY_ORIENTATION:
			return EventType::DisplayOrientation;
		case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
			return EventType::DisplayHaltDrawing;
		case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
			return EventType::DisplayResumeDrawing;
		case ALLEGRO_EVENT_DISPLAY_CONNECTED:
			return EventType::DisplayConnected;
		case ALLEGRO_EVENT_DISPLAY_DISCONNECTED:
			return EventType::DisplayDisconnected;
		default:
			return EventType::Unknown;
		}

	}
	EventSource Event::Source() const {

		return EventSource(_ev.any.source);

	}
	double Event::Timestamp() const {

		return _ev.any.timestamp;

	}
	ALLEGRO_EVENT* Event::AlPtr() {

		return &_ev;

	}

}