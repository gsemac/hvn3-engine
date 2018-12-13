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
#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/core/Runner.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/Engine.h"
#include "hvn3/core/IGameManager.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/events/Event.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/io/DisplayListener.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/io/MouseMutator.h"
#include "hvn3/io/Keyboard.h"
#include "hvn3/io/KeyboardMutator.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/rooms/RoomBase.h"
#include "hvn3/views/IViewManager.h"
#ifdef HVN3_DEBUG 
#include "hvn3/io/Console.h"
#endif

namespace hvn3 {
	namespace System {

		Runner::Runner(Context context) :
			_context(context),
			_timer(1.0 / context.Get<GAME_MANAGER>().Properties().FrameRate),
			_graphics(context.Get<GAME_MANAGER>().Display().BackBuffer()),
			_fps_counter(0.25) {

			// Make sure that the required framework components have been initialized.
			Engine::RequireComponent(EngineComponent::Core | EngineComponent::IO);

			// Take note of the full-screen state of the display (used for listening to full-screen changes).
			_display_was_fullscreen = context.Get<GAME_MANAGER>().Display().IsFullscreen();

			// In case any bitmaps were created before the display, tie them to the current display.
			Graphics::Bitmap::ConvertMemoryBitmapsToVideoBitmaps();

			// Initialize the event queue.
			_event_queue.AddEventSource(context.Get<GAME_MANAGER>().Display().GetEventSource());
			_event_queue.AddEventSource(_timer.EventSource());
			_event_queue.AddEventSource(MouseMutator().GetEventSource());
			_event_queue.AddEventSource(KeyboardMutator().GetEventSource());

			// Initialize other members.
			_allow_redraw = true;
			_exit_loop = false;
			_default_font = nullptr;
			_last_room = nullptr;

		}
		Runner::~Runner() {

			// Dispose of resources.
			if (_default_font)
				delete _default_font;
			_default_font = nullptr;

			Engine::ReleaseComponent(EngineComponent::Core | EngineComponent::IO);

		}
		void Runner::OnDraw(DrawEventArgs& e) {

			if (!_context.Get<ROOM_MANAGER>().IsRoomNull())

				// Render the active Scene.
				_context.Get<ROOM_MANAGER>().OnDraw(e);

			else {

				// Draw placeholder graphics.
				SizeI size = _context.Get<GAME_MANAGER>().Display().Size();
				e.Graphics().DrawText(Math::Round(size.width / 2.0f), Math::Round(size.height / 2.0f), _properties().DisplayTitle.c_str(), *SystemFont(), Color::White, Alignment::Center);

			}

			// If running in debug mode, draw the FPS counter.
			if (_properties().DebugMode)
				DrawFps();

		}
		void Runner::OnUpdate(UpdateEventArgs& e) {

			if (!_context.Get<ROOM_MANAGER>().IsRoomNull() && !_isFrozen() && _frames_skipped++ <= _properties().MaxFrameSkip) {

				// Reset the delta timer.
				_delta_timer.Reset();

				// Update the active room.
				_context.Get<ROOM_MANAGER>().OnUpdate(e);

				// For any keys held, dispatch the appropriate event to all keyboard listeners.
				if (Keyboard::KeyDown(Key::Any))
					KeyboardMutator().DispatchAllKeyDownEvents();

				// For any mouse buttons held, dispatch the appropriate event to all mouse listeners.
				MouseMutator().DispatchAllMouseDownEvents();

				// If the fullscreen state of the display has changed, emit a DisplaySizeChanged event.
				if (_display_was_fullscreen != _context.Get<GAME_MANAGER>().Display().IsFullscreen()) {
					//_context.GetRooms().CurrentRoom()->OnDisplaySizeChanged(DisplaySizeChangedEventArgs(_properties().DisplaySize, _display.Size(), &_display));
					_display_was_fullscreen = _context.Get<GAME_MANAGER>().Display().IsFullscreen();
				}

#if defined(HVN3_DEBUG) && defined(HVN3_DEBUG_CONSOLE)
				// Draw stats in the console window in debug mode. Limiting rate helps with flickering.
				if (_properties().DebugMode) {
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

			if (_properties().FixedFrameRate)
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

			// Allows the game loop to be resumed if it was stopped previously.
			_exit_loop = false;

			// Render once before starting the loop so there's something to see if the framerate is low.
			OnRedraw();

			// Start the timer.
			if (_properties().FixedFrameRate)
				_timer.Start();

			// Start the frame delta timer.
			_delta_timer.Start();

			// Run the update loop.
			while (!_exit_loop)
				WaitForEvent();

			// Call the destructor so resources can be disposed before the framework is shut down.
			// #todo Don't do this, because if this happens the loop can't be resumed later
			Runner::~Runner();

		}
		void Runner::Stop() {
			_exit_loop = true;
		}
		void Runner::DrawFps() {

			// Update the FPS counter.
			_fps_counter.NextFrame();

			// Get the average FPS at this instant.
			double fps = Math::Round(_fps_counter.AverageFps());
			if (_properties().FixedFrameRate)
				fps = Math::Min(fps, static_cast<double>(_properties().FrameRate));

			// Draw the FPS.
			std::stringstream ss;
			ss << fps << " FPS";
			_graphics.DrawText(11, 11, ss.str().c_str(), *SystemFont(), Color::Black);
			_graphics.DrawText(10, 10, ss.str().c_str(), *SystemFont(), Color::White);

		}
		void Runner::OnTimerTick(Event& ev) {

			// Did the room change? If so, reset the delta timer.
			// This prevents having a large delta when a room has taken a while to load.
			if (!_context.Get<ROOM_MANAGER>().IsRoomNull() && _last_room != &*_context.Get<ROOM_MANAGER>().Room()) {
				_delta_timer.Reset();
				_last_room = &*_context.Get<ROOM_MANAGER>().Room();
			}

			// Update the state of the game.
			OnUpdate(UpdateEventArgs(_delta_timer.SecondsElapsed()));

			// Update the mouse position.
			_recalculateMousePosition();

			// Unset all pressed/released keys so the values will be false until next triggered.
			KeyboardMutator().ResetKeyStates(true, true, false);

			// Unset all pressed/released mouse buttons so the values will be false until next triggered.
			MouseMutator().ResetButtonStates(true, true, false);

			// Allow redrawing, since actors have been updated.
			_allow_redraw = true;

		}
		void Runner::OnDisplayClose(Event& ev) {

			_exit_loop = true;

		}
		void Runner::OnKeyDown(Event& ev) {

			// Set the key that was pressed.
			int keycode = ev.AlPtr()->keyboard.keycode;
			KeyboardMutator con;
			con.SetKeyState(keycode, true);
			if (Keyboard::KeyPressed(keycode))
				con.DispatchEvent(KeyPressedEventArgs((Key)ev.AlPtr()->keyboard.keycode));

			// Toggle fullscreen if F11 is pressed.
			if (ev.AlPtr()->keyboard.keycode == ALLEGRO_KEY_F11)
				_context.Get<GAME_MANAGER>().Display().SetFullscreen(!_context.Get<GAME_MANAGER>().Display().IsFullscreen());

			// Exit the loop if the escape key has been pressed.
			if (ev.AlPtr()->keyboard.keycode == ALLEGRO_KEY_ESCAPE && _properties().ExitWithEscapeKey)
				_exit_loop = true;

		}
		void Runner::OnKeyUp(Event& ev) {

			// Unset the key that was pressed.
			KeyboardMutator con;
			con.SetKeyState(ev.AlPtr()->keyboard.keycode, false);
			con.DispatchEvent(KeyUpEventArgs((Key)ev.AlPtr()->keyboard.keycode));

		}
		void Runner::OnKeyChar(Event& ev) {

			// Documentation:
			// https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_char

			// Note that non-printable key codes like arrows, delete, backspace, etc. are also returned here.

			KeyboardMutator con;
			int32_t character = ev.AlPtr()->keyboard.unichar;

			if (Keyboard::IsPrintableChar(character))
				con.SetLastChar(ev.AlPtr()->keyboard.unichar);

			con.DispatchEvent(KeyCharEventArgs(static_cast<Key>(ev.AlPtr()->keyboard.keycode), ev.AlPtr()->keyboard.unichar));

			//if (ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_ENTER &&
			//	ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_PAD_ENTER &&
			//	ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_BACKSPACE &&
			//	ev.AlPtr()->keyboard.keycode != ALLEGRO_KEY_DELETE &&
			//	!(ev.AlPtr()->keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) &&
			//	ev.AlPtr()->keyboard.unichar > 0) {}

		}
		void Runner::OnMouseButtonDown(Event& ev) {

			MouseMutator con;
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
				con.DispatchEvent(MousePressedEventArgs(button, Mouse::DisplayPosition(), Mouse::Position(), Mouse::ButtonDoubleClicked(button) ? 2 : 1));

		}
		void Runner::OnMouseButtonUp(Event& ev) {

			MouseMutator con;
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

			con.DispatchEvent(MouseReleasedEventArgs(button, Mouse::DisplayPosition(), Mouse::Position()));

		}
		void Runner::OnMouseAxes(Event& ev) {

			if (!_isFrozen()) {

				MouseMutator con;
				ALLEGRO_MOUSE_EVENT& mouse = ev.AlPtr()->mouse;

				// Set the Mouse' position relative to the display.
				con.SetDisplayPosition(mouse.x, mouse.y);

				// Calculate the mouse position relative to the view that it's in.
				_recalculateMousePosition();

				// Dispatch event to all mouse listeners.
				con.DispatchEvent(MouseMoveEventArgs(Mouse::DisplayPosition(), Mouse::Position()));

				// If the scroll wheel was moved, set the scroll state.
				if (mouse.dw != 0 || mouse.dz != 0) {
					con.SetScrollState(mouse.dz < 0, mouse.dz > 0);
					con.DispatchEvent(MouseScrollEventArgs(Mouse::DisplayPosition(), Mouse::Position(), PointI(mouse.w, mouse.z), mouse.dw, mouse.dz));
				}

			}

		}
		void Runner::OnMouseEnterDisplay(Event& ev) {

			OnMouseAxes(ev);

			_track_mouse_outside_display = false;

		}
		void Runner::OnMouseLeaveDisplay(Event& ev) {

			_track_mouse_outside_display = true;

		}
		void Runner::OnDisplayResize(Event& ev) {

			Display& display = _context.Get<GAME_MANAGER>().Display();

			// Store the old size so that we can include it in the event args.
			SizeI old_size = display.Size();

			// Acknowledge the resize.
			al_acknowledge_resize(System::AllegroAdapter::ToDisplay(display));

			// Update the size stored by the display.
			display.SetSize(ev.AlPtr()->display.width, ev.AlPtr()->display.height);

			// Notify all listeners.
			DisplaySizeChangedEventArgs args(&display);
			ListenerCollection<IDisplayListener>::ForEach([&](IDisplayListener* i) {
				i->OnDisplaySizeChanged(args);
				HVN3_CONTINUE;
			});

		}
		void Runner::OnDisplaySwitchOut(Event& ev) {

			Display& display = _context.Get<GAME_MANAGER>().Display();

			display._setFocus(false);

			KeyboardMutator().ResetKeyStates();
			MouseMutator().ResetButtonStates(true, true, true);

			// Notify all listeners.

			KeyboardMutator().DispatchKeyboardLostEvent(KeyboardLostEventArgs());

			DisplayLostEventArgs args(&display);
			ListenerCollection<IDisplayListener>::ForEach([&](IDisplayListener* i) {
				i->OnDisplayLost(args);
				HVN3_CONTINUE;
			});

			// Freeze the game if this property is enabled.
			if (_properties().FreezeWhenLostFocus)
				_delta_timer.Stop();

		}
		void Runner::OnDisplaySwitchIn(Event& ev) {

			Display& display = _context.Get<GAME_MANAGER>().Display();

			display._setFocus(true);

			// Notify all listeners.

			KeyboardMutator().DispatchKeyboardFoundEvent(KeyboardFoundEventArgs());

			DisplayFoundEventArgs args(&display);
			ListenerCollection<IDisplayListener>::ForEach([&](IDisplayListener* i) {
				i->OnDisplayFound(args);
				HVN3_CONTINUE;
			});

			// Un-freeze the game if this property is enabled.
			_delta_timer.Start();

		}
		void Runner::OnRedraw() {

			// Clear the drawing surface with the outside outside color.
			_graphics.Clear(_properties().OutsideColor);

			// Set the Transform according to the scaling mode.
			_applyScalingMode();

			// Draw the game state to the application surface.
			OnDraw(DrawEventArgs(_graphics));

			// Swap out the backbuffer.
			_context.Get<GAME_MANAGER>().Display().Refresh();

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



		bool Runner::_isFrozen() {
			return !_context.Get<GAME_MANAGER>().Display().IsFocused() && _properties().FreezeWhenLostFocus;
		}
		void Runner::_applyScalingMode() {

			// If no scene has been loaded, do nothing.
			if (_context.Get<ROOM_MANAGER>().IsRoomNull())
				return;

			// Get a reference to the current room.
			IRoom& room = *_context.Get<ROOM_MANAGER>().Room();

			// For scaling, we need to consider the largest view, or the size of the room if no views are enabled.
			RectangleF room_region = room.VisiblePort();

			// Set the Transform according to the scaling mode.
			Graphics::Transform scaling_transform;
			RectangleF clipping_rectangle(0.0f, 0.0f, room_region.Width(), room_region.Height());

			SizeI display_size = _context.Get<GAME_MANAGER>().Display().Size();
			Scale display_scale = _context.Get<GAME_MANAGER>().Display().Scale();

			switch (_properties().ScalingMode) {

			case ScalingMode::Full:

				// Stretch drawing to fill up the display.
				scaling_transform.Scale(display_scale);
				clipping_rectangle = RectangleF(0.0f, 0.0f, room_region.Width() * display_scale.XScale(), room_region.Height() * display_scale.YScale());

				break;

			case ScalingMode::Fixed: {

				// Maintain the original scale. If the room is smaller than the display, center it.
				float pos_x = (room_region.Width() < display_size.Width()) ? (display_size.Width() / 2.0f - room_region.Width() / 2.0f) : 0.0f;
				float pos_y = (room_region.Height() < display_size.Height()) ? (display_size.Height() / 2.0f - room_region.Height() / 2.0f) : 0.0f;

				scaling_transform.Translate(pos_x, pos_y);
				clipping_rectangle = RectangleF(pos_x, pos_y, Math::Min(room_region.Width(), (float)display_size.Width()), Math::Min(room_region.Height(), (float)display_size.Height()));

				break;

			}
			case ScalingMode::MaintainAspectRatio: {

				// Stretch drawing as much as possible while maintaining the aspect ratio.
				float scale_factor = Math::Min(display_scale.XScale(), display_scale.YScale());
				float room_width = room_region.Width() * scale_factor;
				float room_height = room_region.Height() * scale_factor;
				float pos_x = (room_width < display_size.Width()) ? (display_size.Width() / 2.0f - room_width / 2.0f) : 0.0f;
				float pos_y = (room_height < display_size.Height()) ? (display_size.Height() / 2.0f - room_height / 2.0f) : 0.0f;

				scaling_transform.Scale(scale_factor, scale_factor);
				scaling_transform.Translate(pos_x, pos_y);
				clipping_rectangle = RectangleF(pos_x, pos_y, Math::Min(room_width, (float)display_size.Width()), Math::Min(room_height, (float)display_size.Height()));

				break;
			}
			}

			_graphics.SetTransform(scaling_transform);
			_graphics.SetClip(clipping_rectangle);

		}
		void Runner::_recalculateMousePosition() {

			// #todo fix mouse position scaling

			// If no scene has been loaded, do nothing.
			if (_context.Get<ROOM_MANAGER>().IsRoomNull())
				return;

			IRoom& room = *_context.Get<ROOM_MANAGER>().Room();

			if (room.Views().Count() > 0) {

				// If the mouse is inside of a View, position it relative to the View (where 0, 0 is the top left of the View).
				for (size_t i = room.Views().Count(); i-- > 0u;) {

					const View& view = room.Views().At(i);

					// If the View is disabled or doesn't track the mouse position, do nothing.
					if (!view.MouseTrackingEnabled() || !view.Enabled())
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
					pos = Math::Geometry::PointRotate(pos, view.Region().Midpoint() - view.Position(), -view.Angle());

					// Translate it according to the view's offset.
					pos += view.Position();

					// Set the mouse position to the new position.
					MouseMutator().SetPosition(pos.X(), pos.Y());

					// Note that this only needs to be done once, for the top-most view that the mouse is on, so the loop can be exited early.
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

				if (_properties().ScalingMode != ScalingMode::Fixed)
					Scale(SizeF(room.Width(), room.Height()), _graphics.Clip().Size()).ScalePoint(pos);

				// Set the new mouse position.
				MouseMutator().SetPosition(pos.X(), pos.Y());

			}

		}
		GameProperties& Runner::_properties() {

			return _context.Get<GAME_MANAGER>().Properties();

		}

	}
}