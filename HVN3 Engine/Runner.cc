#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <xmmintrin.h>
#include <memory>
#include <sstream>
#include "Runner.h"
#include "Font.h"
#include "RoomBase.h"
#include "io/Mouse.h"
#include "io/Keyboard.h"
#include "DrawEventArgs.h"
#include "UpdateEventArgs.h"
#include "RoomController.h"

Runner::Runner(::Properties& properties, RoomManager& room_manager) :
	_properties(properties),
	__timer(1.0f / properties.FPS),
	__display(properties.DisplaySize.Width(), properties.DisplaySize.Height(), properties.DisplayTitle.c_str(), properties.DisplayFlags),
	__graphics(__display.BackBuffer()),
	_room_manager(room_manager) {

	// Create the display, and initialize its parameters.
	if (properties.Fullscreen)
		__display.SetFullscreen(true);

	// Initialize the event queue.
	__event_queue.AddEventSource(__display.EventSource());
	__event_queue.AddEventSource(__timer.EventSource());
	__event_queue.AddEventSource(Mouse::EventSource());
	__event_queue.AddEventSource(Keyboard::EventSource());

	// Initialize mouse parameters.
	if (!properties.DisplayCursor)
		Mouse::HideCursor();

	// Initialize other member variables.
	__allow_redraw = true;
	__exit_loop = false;
	__default_font = nullptr;

}
Runner::~Runner() {

	// Dispose of resources.

	if (__default_font)
		delete __default_font;
	__default_font = nullptr;

	//if (__graphics)
	//	delete __graphics;
	//__graphics = nullptr;

}
void Runner::Draw(DrawEventArgs& e) {

	if (_room_manager.RoomCount() > 0)

		// Render the active Scene.
		_room_manager.Draw(e);

	else
		// Draw placeholder graphics.
		e.Graphics().DrawText(Round(__display.Width() / 2.0f), Round(__display.Height() / 2.0f), Properties().DisplayTitle.c_str(), *SystemFont(), Color::White, Alignment::Center);

	// If running in debug mode, draw the FPS counter.
	if (Properties().DebugMode)
		DrawFPS();

}
void Runner::Update(UpdateEventArgs& e) {

	// Update the active scene.
	if (_room_manager.RoomCount() > 0 && (!Properties().FreezeWhenLostFocus || __display.HasFocus()) && __frames_skipped++ <= Properties().MaxFrameSkip)
		_room_manager.Update(e);

}
void Runner::WaitForEvent() {

	// Wait for the next event.
	Event ev;
	__event_queue.WaitForEvent(ev);

	// Handle the next event depending on its type.
	switch (ev.Type()) {

	case EventType::Timer:
		OnTimerTick(ev);
		break;

	case EventType::DisplayClose:
		OnDisplayClose(ev);
		break;

	case EventType::KeyDown:
		OnKeyDown(ev);
		break;

	case EventType::KeyUp:
		OnKeyUp(ev);
		break;

	case EventType::KeyChar:
		OnKeyChar(ev);
		break;

	case EventType::MouseButtonDown:
		OnMouseButtonDown(ev);
		break;

	case EventType::MouseButtonUp:
		OnMouseButtonUp(ev);
		break;

	case EventType::MouseAxes:
		OnMouseAxes(ev);
		break;

	case EventType::MouseEnterDisplay:
		OnMouseEnterDisplay(ev);
		break;

	case EventType::MouseLeaveDisplay:
		OnMouseLeaveDisplay(ev);
		break;

	case EventType::DisplayResize:
		OnDisplayResize(ev);
		break;

	case EventType::DisplaySwitchOut:
		OnDisplaySwitchOut(ev);
		break;

	case EventType::DisplaySwitchIn:
		OnDisplaySwitchIn(ev);
		break;

	}

	// Redraw the display surface when the event queue is empty.
	if (__allow_redraw && __event_queue.IsEmpty())
		OnRedraw();

}
void Runner::Loop() {

	// Render once before starting the loop so there's something to see if the framerate is low.
	OnRedraw();

	// Start the timer.
	__timer.Start();

	// Run the update loop.
	while (!__exit_loop)
		WaitForEvent();

	// Call the destructor so resources can be disposed before the framework is shut down.
	Runner::~Runner();

}
void Runner::DrawFPS() {

	// Initialize variables.
	static float fps_buf[60];
	static float fps_sum = 0.0f;
	static int buf_index = 0;
	static Stopwatch fps_timer(true);

	// Calculate the average FPS over 60 frames.
	int new_fps = (float)(1.0f / fps_timer.SecondsElapsed());
	fps_sum -= fps_buf[buf_index];
	fps_sum += new_fps;
	fps_buf[buf_index] = new_fps;
	if (++buf_index == 60)
		buf_index = 0;

	// Draw the FPS.
	std::stringstream ss;
	ss << (int)(std::min)(fps_sum / 60.0f, Properties().FPS) << " FPS";
	__graphics.DrawText(11, 11, ss.str().c_str(), *SystemFont(), Color::Black);
	__graphics.DrawText(10, 10, ss.str().c_str(), *SystemFont(), Color::White);

	// Reset the FPS timer.
	fps_timer.Reset(true);

}

void Runner::OnTimerTick(Event& ev) {

	// Initialize the delta timer. At some point, the frame timer could be used for this.
	static Stopwatch delta_timer(true);

	// Update the state of the game.
	Update(UpdateEventArgs(delta_timer.SecondsElapsed()));

	// Update the mouse position.
	RecalculateMousePosition();

	// Unset all pressed/released keys so the values will be false until next triggered.
	Keyboard::StateAccessor::ResetKeyStates(true, true, false);

	// Unset all pressed/released mouse buttons so the values will be false until next triggered.
	Mouse::StateAccessor::ResetButtonStates(true, true, false);

	// Reset the delta timer.
	delta_timer.Reset(true);

	// Allow redrawing, since actors have been updated.
	__allow_redraw = true;

}
void Runner::OnDisplayClose(Event& ev) {

	__exit_loop = true;

}
void Runner::OnKeyDown(Event& ev) {

	// Set the key that was pressed.
	Keyboard::StateAccessor::SetKeyState(ev.AlPtr()->keyboard.keycode, true);

	// Toggle fullscreen if F11 is pressed.
	if (ev.AlPtr()->keyboard.keycode == ALLEGRO_KEY_F11)
		__display.SetFullscreen(!__display.IsFullscreen());

	// Exit the loop if the escape key has been pressed.
	if (ev.AlPtr()->keyboard.keycode == ALLEGRO_KEY_ESCAPE && Properties().ExitWithEscapeKey)
		__exit_loop = true;

}
void Runner::OnKeyUp(Event& ev) {

	// Unset the key that was pressed.
	Keyboard::StateAccessor::SetKeyState(ev.AlPtr()->keyboard.keycode, false);

}
void Runner::OnKeyChar(Event& ev) {

	if (ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_ENTER &&
		ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_PAD_ENTER &&
		ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_BACKSPACE &&
		ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_DELETE &&
		!(ev.AlPtr()->keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) &&
		ev.AlPtr()->keyboard.unichar > 0)
		Keyboard::StateAccessor::SetLastChar(ev.AlPtr()->keyboard.unichar);

}
void Runner::OnMouseButtonDown(Event& ev) {

	switch (ev.AlPtr()->mouse.button) {

	case 1:
		Mouse::StateAccessor::SetButtonState(MB_LEFT, true);
		break;

	case 2:
		Mouse::StateAccessor::SetButtonState(MB_RIGHT, true);
		break;

	case 3:
		Mouse::StateAccessor::SetButtonState(MB_MIDDLE, true);
		break;

	}

}
void Runner::OnMouseButtonUp(Event& ev) {

	switch (ev.AlPtr()->mouse.button) {

	case 1:
		Mouse::StateAccessor::SetButtonState(MB_LEFT, false);
		break;

	case 2:
		Mouse::StateAccessor::SetButtonState(MB_RIGHT, false);
		break;

	case 3:
		Mouse::StateAccessor::SetButtonState(MB_MIDDLE, false);
		break;

	}

}
void Runner::OnMouseAxes(Event& ev) {

	if (!Properties().FreezeWhenLostFocus || __display.HasFocus()) {

		// Set the Mouse' position relative to the display.
		Mouse::StateAccessor::SetDisplayPosition(ev.AlPtr()->mouse.x, ev.AlPtr()->mouse.y);

		// Calculate the mouse position relative to the view that it's in.
		RecalculateMousePosition();

		// If the scroll wheel was moved, set the scroll state.
		static int scroll_position = 0;
		Mouse::StateAccessor::SetScrollState(ev.AlPtr()->mouse.z < scroll_position, ev.AlPtr()->mouse.z > scroll_position);
		scroll_position = ev.AlPtr()->mouse.z;

	}

}
void Runner::OnMouseEnterDisplay(Event& ev) {

	OnMouseAxes(ev);

}
void Runner::OnMouseLeaveDisplay(Event& ev) {

	// Set the Mouse coordinates to null.
	//Mouse::X = -1;
	//Mouse::Y = -1;

}
void Runner::OnDisplayResize(Event& ev) {

	al_acknowledge_resize(__display.AlPtr());
	__display.Resize(ev.AlPtr()->display.width, ev.AlPtr()->display.height);

}
void Runner::OnDisplaySwitchOut(Event& ev) {

	Display::StateAccessor(&__display).SetFocus(false);
	Keyboard::StateAccessor::ResetKeyStates();

}
void Runner::OnDisplaySwitchIn(Event& ev) {

	Display::StateAccessor(&__display).SetFocus(true);

}
void Runner::OnRedraw() {

	// Clear the drawing surface with the outside outside color.
	__graphics.Clear(Properties().OutsideColor);

	// Set the Transform according to the scaling mode.
	ApplyScalingMode();

	// Draw the game state to the application surface.
	Draw(DrawEventArgs(__graphics));

	// Swap out the backbuffer.
	__display.Refresh();

	// Redrawing will be disabled until the event queue is cleared.
	__allow_redraw = false;

	// Reset the skipped frame count.
	__frames_skipped = 0;

}

const Font* Runner::SystemFont() {

	if (!__default_font)
		__default_font = new Font(Font::BuiltIn());

	return __default_font;

}

const Properties& Runner::Properties() const {

	return _properties;

}
RoomBase& Runner::CurrentRoom() {

	return _room_manager.CurrentRoom();

}

void Runner::ApplyScalingMode() {

	// If no scene has been loaded, do nothing.
	if (_room_manager.RoomCount() <= 0)
		return;

	RoomBase& room = _room_manager.CurrentRoom();

	// Set the Transform according to the scaling mode.
	Drawing::Transform scaling_transform;
	Rectangle clipping_rectangle(0.0f, 0.0f, room.Width(), room.Height());

	switch (Properties().ScalingMode) {

	case ScalingMode::Full:
		// Stretch drawing to fill up the Display.
		scaling_transform.Scale(__display.Scale());
		clipping_rectangle = Rectangle(0.0f, 0.0f, room.Width() * __display.Scale().XScale(), room.Height() * __display.Scale().YScale());
		break;

	case ScalingMode::Fixed:
		// Center drawing while maintaining original scale.
		scaling_transform.Translate(__display.Width() / 2.0f - room.Width() / 2.0f, __display.Height() / 2.0f - room.Height() / 2.0f);
		clipping_rectangle = Rectangle(__display.Width() / 2.0f - room.Width() / 2.0f, __display.Height() / 2.0f - room.Height() / 2.0f, room.Width(), room.Height());
		break;

	case ScalingMode::MaintainAspectRatio:
		// Stretch drawing as much as possible while maintaining the aspect ratio.
		float scale_factor = Min(__display.Scale().XScale(), __display.Scale().YScale());
		scaling_transform.Scale(scale_factor, scale_factor);
		scaling_transform.Translate(__display.Width() / 2.0f - room.Width() * scale_factor / 2.0f, __display.Height() / 2.0f - room.Height() * scale_factor / 2.0f);
		clipping_rectangle = Rectangle(__display.Width() / 2.0f - room.Width() * scale_factor / 2.0f, __display.Height() / 2.0f - room.Height() * scale_factor / 2.0f, room.Width() * scale_factor, room.Height() * scale_factor);
		break;

	}

	__graphics.SetTransform(scaling_transform);
	__graphics.SetClip(clipping_rectangle);

}
void Runner::RecalculateMousePosition() {

	// If no scene has been loaded, do nothing.
	if (_room_manager.RoomCount() <= 0)
		return;

	Room& room = _room_manager.CurrentRoom();

	if (room.ViewManager()->ViewCount()) {

		// If the mouse is inside of a View, position it relative to the View (where 0, 0 is the top left of the View).
		for (int i = room.ViewManager()->ViewCount() - 1; i >= 0; --i) {

			// Get a reference to the view so it's easy to access.
			View& view = room.ViewManager()->ViewAt(i);

			// If the View is disabled or doesn't track the mouse position, do nothing.
			if (!view.TracksMouse() || !view.Enabled())
				continue;

			// If the mouse is not inside of the View's viewport, do nothing.
			Point pos = Mouse::DisplayPosition();
			Point port_p1 = view.Port().TopLeft();
			Point port_p2 = view.Port().BottomRight();
			__graphics.GetTransform().TransformPoint(port_p1);
			__graphics.GetTransform().TransformPoint(port_p2);
			Rectangle viewport(port_p1, port_p2);
			if (!PointIn(pos, viewport))
				continue;

			// Get the scale of the actual viewport relative to the view's port, as well as the view's scale.
			Scale port_scale = Scale(viewport, view.Port());
			Scale view_scale(view.Scale());

			// Get the mouse position relative to the viewport.
			pos -= port_p1;

			// Scale it according to the view scale.
			(view_scale * port_scale).Inverse().ScalePoint(pos);

			// Rotate it against the view's angle.
			PointRotate(pos, view.Port().Midpoint(), -view.Angle());

			// Translate it according to the view's offset.
			pos += view.Position();

			Mouse::StateAccessor::SetPosition(pos.X(), pos.Y());
			break;

		}

	}
	else {

		// If Views are not used, set the mouse position to its position relative to the display.
		Point pos = Mouse::DisplayPosition();

		// If the mouse is outside of the clipping area, don't track it.
		if (!PointIn(pos, __graphics.Clip()))
			return;

		// Set the position relative to the clipping area.
		pos -= __graphics.Clip().TopLeft();
		Scale(Size(room.Width(), room.Height()), __graphics.Clip().Size()).ScalePoint(pos);

		Mouse::StateAccessor::SetPosition(pos.X(), pos.Y());

	}

}