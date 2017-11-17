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
#include "core/Runner.h"
#include "fonts/Font.h"
#include "rooms/RoomBase.h"
#include "io/Mouse.h"
#include "io/Keyboard.h"
#include "core/DrawEventArgs.h"
#include "core/UpdateEventArgs.h"
#include "rooms/RoomController.h"
#include "events/Event.h"
#include "graphics/DisplayController.h"
#include "io/KeyboardController.h"
#include "io/MouseController.h"
#ifdef HVN3_DEBUG 
#include "io/Console.h"
#endif

namespace hvn3 {
	namespace System {

		Runner::Runner(System::Properties* properties, RoomManager* room_manager) :
			_properties(properties),
			_timer(1.0f / properties->FrameRate),
			_display(properties->DisplaySize.Width(), properties->DisplaySize.Height(), properties->DisplayTitle.c_str(), properties->DisplayFlags),
			_graphics(_display.BackBuffer()),
			_room_manager(room_manager),
			_fps_counter(0.25) {

			// Create the display, and initialize its parameters.
			if (properties->StartFullscreen)
				_display.SetFullscreen(true);
			_display_was_fullscreen = _display.IsFullscreen();

			// Initialize the event queue.
			_event_queue.AddEventSource(_display.EventSource());
			_event_queue.AddEventSource(_timer.EventSource());
			_event_queue.AddEventSource(MouseController().GetEventSource());
			_event_queue.AddEventSource(KeyboardController().GetEventSource());

			// Initialize mouse parameters.
			if (!properties->DisplayCursor)
				Mouse::HideCursor();

			// Initialize other member variables.
			_allow_redraw = true;
			_exit_loop = false;
			_default_font = nullptr;

		}
		Runner::~Runner() {

			// Dispose of resources.
			if (_default_font)
				delete _default_font;
			_default_font = nullptr;

		}
		void Runner::Draw(DrawEventArgs& e) {

			if (_room_manager->RoomCount() > 0)

				// Render the active Scene.
				_room_manager->Draw(e);

			else
				// Draw placeholder graphics.
				e.Graphics().DrawText(Math::Round(_display.Width() / 2.0f), Math::Round(_display.Height() / 2.0f), Properties().DisplayTitle.c_str(), SystemFont(), Color::White, Alignment::Center);

			// If running in debug mode, draw the FPS counter.
			if (Properties().DebugMode)
				DrawFps();

		}
		void Runner::Update(UpdateEventArgs& e) {

			if (_room_manager->RoomCount() > 0 && (!Properties().FreezeWhenLostFocus || _display.HasFocus()) && _frames_skipped++ <= Properties().MaxFrameSkip) {

				// Reset the delta timer.
				_delta_timer.Reset();

				// Update the active room.
				_room_manager->Update(e);

				// For any keys held, dispatch the appropriate event to all keyboard listeners.
				if (Keyboard::KeyDown(Key::Any))
					KeyboardController().DispatchAllKeyDownEvents();

				// For any mouse buttons held, dispatch the appropriate event to all mouse listeners.
				MouseController().DispatchAllMouseDownEvents();

				// If the fullscreen state of the display has changed, emit a DisplaySizeChanged event.
				if (_display_was_fullscreen != _display.IsFullscreen()) {
					_room_manager->CurrentRoom()->OnDisplaySizeChanged(DisplaySizeChangedEventArgs(Properties().DisplaySize, _display.Size(), &_display));
					_display_was_fullscreen = _display.IsFullscreen();
				}

#if defined(HVN3_DEBUG) && defined(HVN3_DEBUG_CONSOLE)
				// Draw stats in the console window in debug mode. Limiting rate helps with flickering.
				if (Properties().DebugMode) {
					Console::Clear();
					Console::WriteLine("------------------------------");
					Console::WriteLine(" Room");
					Console::WriteLine("------------------------------");
					Console::Write("Instances: ");
					Console::WriteLine(_room_manager.CurrentRoom()->Objects()->InstanceCount());
					if (_room_manager.CurrentRoom()->Collisions()) {
						Console::Write("\nCollision Bodies: ");
					}
					Console::WriteLine("------------------------------");
					Console::WriteLine(" Global");
					Console::WriteLine("------------------------------");
					Console::Write("Mouse: ");
					Console::WriteLine(Mouse::Position());
				}
#endif

			}

		}
		void Runner::WaitForEvent() {

			// Wait for the next event.
			Event ev;
			bool got_event = true;

			if (Properties().FixedFrameRate)
				_event_queue.WaitForEvent(ev);
			else if (!_event_queue.GetNextEvent(ev)) {
				OnTimerTick(ev);
				got_event = false;
			}

			if (got_event) {

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

			}

			// Redraw the display surface when the event queue is empty.
			if (_allow_redraw && _event_queue.IsEmpty())
				OnRedraw();

		}
		void Runner::Loop() {

			// Render once before starting the loop so there's something to see if the framerate is low.
			OnRedraw();

			// Start the timer.
			if (Properties().FixedFrameRate)
				_timer.Start();

			// Start the frame delta timer.
			_delta_timer.Start();

			// Run the update loop.
			while (!_exit_loop)
				WaitForEvent();

			// Call the destructor so resources can be disposed before the framework is shut down.
			Runner::~Runner();

		}
		void Runner::DrawFps() {

			// Update the FPS counter.
			_fps_counter.NextFrame();

			// Get the average FPS at this instant.
			double fps = Math::Round(_fps_counter.AverageFps());
			if (_properties->FixedFrameRate)
				fps = Math::Min(fps, static_cast<double>(_properties->FrameRate));

			// Draw the FPS.
			std::stringstream ss;
			ss << fps << " FPS";
			_graphics.DrawText(11, 11, ss.str().c_str(), SystemFont(), Color::Black);
			_graphics.DrawText(10, 10, ss.str().c_str(), SystemFont(), Color::White);

		}

		void Runner::OnTimerTick(Event& ev) {

			// Update the state of the game.
			Update(UpdateEventArgs(_delta_timer.SecondsElapsed()));

			// Update the mouse position.
			RecalculateMousePosition();

			// Unset all pressed/released keys so the values will be false until next triggered.
			KeyboardController().ResetKeyStates(true, true, false);

			// Unset all pressed/released mouse buttons so the values will be false until next triggered.
			MouseController().ResetButtonStates(true, true, false);

			// Allow redrawing, since actors have been updated.
			_allow_redraw = true;

		}
		void Runner::OnDisplayClose(Event& ev) {

			_exit_loop = true;

		}
		void Runner::OnKeyDown(Event& ev) {

			// Set the key that was pressed.
			int keycode = ev.AlPtr()->keyboard.keycode;
			KeyboardController con;
			con.SetKeyState(keycode, true);
			if (Keyboard::KeyPressed(keycode))
				con.DispatchEvent(KeyPressedEventArgs((Key)ev.AlPtr()->keyboard.keycode));

			// Toggle fullscreen if F11 is pressed.
			if (ev.AlPtr()->keyboard.keycode == ALLEGRO_KEY_F11)
				_display.SetFullscreen(!_display.IsFullscreen());

			// Exit the loop if the escape key has been pressed.
			if (ev.AlPtr()->keyboard.keycode == ALLEGRO_KEY_ESCAPE && Properties().ExitWithEscapeKey)
				_exit_loop = true;

		}
		void Runner::OnKeyUp(Event& ev) {

			// Unset the key that was pressed.
			KeyboardController con;
			con.SetKeyState(ev.AlPtr()->keyboard.keycode, false);
			con.DispatchEvent(KeyUpEventArgs((Key)ev.AlPtr()->keyboard.keycode));

		}
		void Runner::OnKeyChar(Event& ev) {

			if (ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_ENTER &&
				ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_PAD_ENTER &&
				ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_BACKSPACE &&
				ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_DELETE &&
				!(ev.AlPtr()->keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) &&
				ev.AlPtr()->keyboard.unichar > 0) {

				KeyboardController con;
				con.SetLastChar(ev.AlPtr()->keyboard.unichar);
				con.DispatchEvent(KeyCharEventArgs((Key)ev.AlPtr()->keyboard.keycode, ev.AlPtr()->keyboard.unichar));

			}

		}
		void Runner::OnMouseButtonDown(Event& ev) {

			MouseController con;
			MouseButton button;

			switch (ev.AlPtr()->mouse.button) {

			case 1:
				button = MouseButton::Left;
				break;

			case 2:
				button = MouseButton::Right;
				break;

			case 3:
				button = MouseButton::Middle;
				break;

			}

			con.SetButtonState(button, true);

			if (Mouse::ButtonPressed(button))
				con.DispatchEvent(MousePressedEventArgs(button, Mouse::ButtonDoubleClicked(button) ? 2 : 1));

		}
		void Runner::OnMouseButtonUp(Event& ev) {

			MouseController con;
			MouseButton button;

			switch (ev.AlPtr()->mouse.button) {

			case 1:
				button = MouseButton::Left;
				break;

			case 2:
				button = MouseButton::Right;
				break;

			case 3:
				button = MouseButton::Middle;
				break;

			}

			con.SetButtonState(button, false);

			con.DispatchEvent(MouseUpEventArgs(button));

		}
		void Runner::OnMouseAxes(Event& ev) {

			if (!Properties().FreezeWhenLostFocus || _display.HasFocus()) {

				MouseController con;

				// Set the Mouse' position relative to the display.
				con.SetDisplayPosition(ev.AlPtr()->mouse.x, ev.AlPtr()->mouse.y);

				// Calculate the mouse position relative to the view that it's in.
				RecalculateMousePosition();

				// Dispatch event to all mouse listeners.
				con.DispatchEvent(MouseMoveEventArgs());

				// If the scroll wheel was moved, set the scroll state.
				ALLEGRO_MOUSE_EVENT& mouse = ev.AlPtr()->mouse;
				if (mouse.dw != 0 || mouse.dz != 0) {
					con.SetScrollState(mouse.dz < 0, mouse.dz > 0);
					con.DispatchEvent(MouseScrollEventArgs(mouse.w, mouse.z, mouse.dw, mouse.dz));
				}

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

			// Store the old size so that we can include it in the event args.
			SizeI old_size(_display.Width(), _display.Height());

			// Acknowledge the resize.
			al_acknowledge_resize(_display.AlPtr());

			// Resize the display.
			_display.Resize(ev.AlPtr()->display.width, ev.AlPtr()->display.height);

			// Trigger the "OnDisplaySizeChanged" event for the current room.
			if (_room_manager->RoomCount() > 0)
				_room_manager->CurrentRoom()->OnDisplaySizeChanged(
					DisplaySizeChangedEventArgs(old_size, SizeI(_display.Width(), _display.Height()), &_display)
				);

		}
		void Runner::OnDisplaySwitchOut(Event& ev) {

			System::DisplayController(&_display).SetFocus(false);
			KeyboardController().ResetKeyStates();

			if (Properties().FreezeWhenLostFocus)
				_delta_timer.Stop();

		}
		void Runner::OnDisplaySwitchIn(Event& ev) {

			System::DisplayController(&_display).SetFocus(true);

			_delta_timer.Start();

		}
		void Runner::OnRedraw() {

			// Clear the drawing surface with the outside outside color.
			_graphics.Clear(Properties().OutsideColor);

			// Set the Transform according to the scaling mode.
			ApplyScalingMode();

			// Draw the game state to the application surface.
			Draw(DrawEventArgs(_graphics));

			// Swap out the backbuffer.
			_display.Refresh();

			// Redrawing will be disabled until the event queue is cleared.
			_allow_redraw = false;

			// Reset the skipped frame count.
			_frames_skipped = 0;

		}

		const Font* Runner::SystemFont() {

			if (!_default_font)
				_default_font = new Font(Font::BuiltIn());

			return _default_font;

		}

		const Properties& Runner::Properties() const {

			return *_properties;

		}
		IRoom* Runner::CurrentRoom() {

			return _room_manager->CurrentRoom();

		}

		void Runner::ApplyScalingMode() {

			// If no scene has been loaded, do nothing.
			if (_room_manager->RoomCount() <= 0)
				return;

			// Get a reference to the current room.
			IRoom& room = *_room_manager->CurrentRoom();

			// For scaling, we need to consider the largest view, or the size of the room if no views are enabled.
			RectangleF room_region = room.GetVisibleRegion();

			// Set the Transform according to the scaling mode.
			Graphics::Transform scaling_transform;
			RectangleF clipping_rectangle(0.0f, 0.0f, room_region.Width(), room_region.Height());

			switch (Properties().ScalingMode) {

			case ScalingMode::Full:

				// Stretch drawing to fill up the display.
				scaling_transform.Scale(_display.Scale());
				clipping_rectangle = RectangleF(0.0f, 0.0f, room_region.Width() * _display.Scale().XScale(), room_region.Height() * _display.Scale().YScale());

				break;

			case ScalingMode::Fixed: {

				// Maintain the original scale. If the room is smaller than the display, center it.
				float pos_x = (room_region.Width() < _display.Width()) ? (_display.Width() / 2.0f - room_region.Width() / 2.0f) : 0.0f;
				float pos_y = (room_region.Height() < _display.Height()) ? (_display.Height() / 2.0f - room_region.Height() / 2.0f) : 0.0f;

				scaling_transform.Translate(pos_x, pos_y);
				clipping_rectangle = RectangleF(pos_x, pos_y, Math::Min(room_region.Width(), (float)_display.Width()), Math::Min(room_region.Height(), (float)_display.Height()));

				break;

			}
			case ScalingMode::MaintainAspectRatio: {

				// Stretch drawing as much as possible while maintaining the aspect ratio.
				float scale_factor = Math::Min(_display.Scale().XScale(), _display.Scale().YScale());
				float room_width = room_region.Width() * scale_factor;
				float room_height = room_region.Height() * scale_factor;
				float pos_x = (room_width < _display.Width()) ? (_display.Width() / 2.0f - room_width / 2.0f) : 0.0f;
				float pos_y = (room_height < _display.Height()) ? (_display.Height() / 2.0f - room_height / 2.0f) : 0.0f;

				scaling_transform.Scale(scale_factor, scale_factor);
				scaling_transform.Translate(pos_x, pos_y);
				clipping_rectangle = RectangleF(pos_x, pos_y, Math::Min(room_width, (float)_display.Width()), Math::Min(room_height, (float)_display.Height()));

				break;
			}
			}

			_graphics.SetTransform(scaling_transform);
			_graphics.SetClip(clipping_rectangle);

		}
		void Runner::RecalculateMousePosition() {

			// #todo fix mouse position scaling

			// If no scene has been loaded, do nothing.
			if (_room_manager->RoomCount() <= 0)
				return;

			IRoom& room = *_room_manager->CurrentRoom();

			if (room.Views()->ViewCount()) {

				// If the mouse is inside of a View, position it relative to the View (where 0, 0 is the top left of the View).
				for (int i = room.Views()->ViewCount() - 1; i >= 0; --i) {

					// Get a reference to the view so it's easy to access.
					View& view = room.Views()->ViewAt(i);

					// If the View is disabled or doesn't track the mouse position, do nothing.
					if (!view.TracksMouse() || !view.Enabled())
						continue;

					// If the mouse is not inside of the View's viewport, do nothing.
					PointF pos = Mouse::DisplayPosition();
					PointF port_p1 = view.Port().TopLeft();
					PointF port_p2 = view.Port().BottomRight();
					_graphics.GetTransform().TransformPoint(port_p1);
					_graphics.GetTransform().TransformPoint(port_p2);
					RectangleF viewport(port_p1, port_p2);
					if (!Math::Geometry::PointIn(pos, viewport))
						continue;

					// Get the scale of the actual viewport relative to the view's port, as well as the view's scale.
					Scale port_scale = Scale(viewport, view.Port());
					Scale view_scale(view.Scale());

					// Get the mouse position relative to the viewport.
					pos -= port_p1;

					// Scale it according to the view scale.
					(view_scale * port_scale).Inverse().ScalePoint(pos);

					// Rotate it against the view's angle.
					pos = Math::Geometry::PointRotate(pos, view.Port().Midpoint(), -view.Angle());

					// Translate it according to the view's offset.
					pos += view.Position();

					MouseController().SetPosition(pos.X(), pos.Y());
					break;

				}

			}
			else {

				// If views are not used, set the mouse position to its position relative to the display.
				PointF pos = Mouse::DisplayPosition();

				// If the mouse is outside of the clipping area, don't track it.
				if (!Math::Geometry::PointIn(pos, _graphics.Clip()))
					return;

				// Set the position relative to the clipping area.
				pos -= _graphics.Clip().TopLeft();

				if (_properties->ScalingMode != ScalingMode::Fixed)
					Scale(SizeF(room.Width(), room.Height()), _graphics.Clip().Size()).ScalePoint(pos);

				// Set the new mouse position.
				MouseController().SetPosition(pos.X(), pos.Y());

			}

		}

	}
}