//#include "hvn3/rooms/Room.h"
//#include "hvn3/objects/ObjectManager.h"
//
//#include <cassert>
//
//namespace hvn3 {
//
//	Room::Room(RoomId id, const SizeI& size) :
//		RoomBase(id, size) {
//
//		_rendering_view = 0;
//		_restart_pending = false;
//
//		_initializeAllManagers();
//
//	}
//	Room::Room(RoomId id, int width, int height) :
//		Room(id, SizeI(width, height)) {
//	}
//	Room::Room(const SizeI& size) :
//		Room(NULL_ROOM_ID, size) {
//	}
//	Room::Room(int width, int height) :
//		Room(NULL_ROOM_ID, width, height) {
//	}
//	void Room::OnUpdate(UpdateEventArgs& e) {
//
//		// Pending restarts should be processed before anything else, so it doesn't do an extra update when resets are initiated outside of it.
//
//		if (_restart_pending) {
//
//			// Reset the state of the room.
//			OnReset();
//
//			RoomCreateEventArgs args;
//
//			// Set-up the room again.
//			OnCreate(args);
//
//			// Disable the restarting pending flag.
//			_restart_pending = false;
//
//		}
//
//		// Update objects (begin).
//		Objects().OnBeginUpdate(e);
//
//		// Update objects (primary).
//		Objects().OnUpdate(e);
//
//		// Update collision manager.
//		Collisions().OnUpdate(e);
//
//		// Update physics manager.
//		Physics().OnUpdate(e);
//
//		// Update views.
//		Views().Update(ViewUpdateEventArgs(e.Delta(), hvn3::SizeI(Width(), Height())));
//
//		// Update backgrounds.
//		Backgrounds().Update(e);
//
//		// Update objects (end).
//		Objects().OnEndUpdate(e);
//
//	}
//	void Room::OnDraw(DrawEventArgs& e) {
//
//		// Save the current graphics state.
//		Graphics::Transform original_tranform(e.Graphics().GetTransform());
//		RectangleF original_clip(e.Graphics().Clip());
//
//		// Each View needs to be drawn separately.
//		if (Local().Get<VIEW_MANAGER>().Count() > 0) {
//
//			for (size_t i = 0; i < Local().Get<VIEW_MANAGER>().Count(); ++i) {
//
//				// Set current view index.
//				_rendering_view = i;
//
//				// If the current view isn't enabled, we don't need to draw it.
//				if (!CurrentView().Enabled())
//					continue;
//
//				// Set the clipping region according to the view port.
//				PointF p1 = CurrentView().Port().TopLeft();
//				PointF p2 = CurrentView().Port().BottomRight();
//				p1 = original_tranform.TransformPoint(p1);
//				p2 = original_tranform.TransformPoint(p2);
//				RectangleF clip(p1, p2);
//				e.Graphics().SetClip(clip);
//
//				// Set transform according to view state.		
//				Graphics::Transform transform = CurrentView().GetTransform();
//				transform.Compose(original_tranform);
//				e.Graphics().SetTransform(transform);
//
//				// Render room contents.
//				OnRender(e);
//
//			}
//
//			// Reset current view to 0.
//			_rendering_view = 0;
//
//		}
//		else
//			// Simply render the room without any consideration for views.
//			OnRender(e);
//
//		// Restore the previous graphics state.
//		e.Graphics().SetTransform(original_tranform);
//		e.Graphics().SetClip(original_clip);
//
//	}
//	void Room::SetBackground(const Background& value) {
//
//		Backgrounds().Clear();
//		Backgrounds().Add(value);
//
//	}
//	const View& Room::CurrentView() const {
//
//		if (Views().Count() == 0)
//			throw System::NullReferenceException();
//
//		return Views().At(_rendering_view);
//
//	}
//	RectangleF Room::VisiblePort() const {
//
//		// Find the largest viewport, or the size of the room if no views are enabled.
//		const View* largest = nullptr;
//		for (size_t i = 0; i < Views().Count(); ++i) {
//			const View* view = &Views().At(i);
//			if (!view->Enabled())
//				continue;
//			if (largest == nullptr || view->Port().Size() > largest->Port().Size())
//				largest = view;
//		}
//
//		if (largest == nullptr)
//			return RoomBase::VisiblePort();
//
//		return largest->Port();
//
//	}
//	RectangleF Room::VisibleRegion() const {
//
//		// Return the visible region given by the current view (if views are enabled).
//		if (Views().Count() > 0)
//			return CurrentView().Region();
//
//		// Otherwise, return a region corresponding to the entire room size.
//		return RoomBase::VisibleRegion();
//
//	}
//
//	void Room::Restart() {
//
//		_restart_pending = true;
//
//	}
//	void Room::OnReset() {
//		RoomBase::OnReset();
//
//		// Reset base (clears all objects).
//		Objects().Clear();
//
//		// Clear the physics manager first, as it may hold references to the collision manager.
//		Physics().Clear();
//
//		// Clear all bodies from the collision manager.
//		Collisions().Clear();
//
//		// Reset the view manager.
//		Views().Clear();
//		_rendering_view = 0;
//
//		// Reset the background manager.
//		Backgrounds().Clear();
//
//	}
//	void Room::OnRender(DrawEventArgs& e) {
//
//		// Clear to background color.
//		e.Graphics().Clear(BackgroundColor());
//
//		// Draw all backgrounds.
//		const View* current_view = Views().Count() > 0 ? &CurrentView() : nullptr;
//		Backgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), current_view, false));
//
//		// Draw all tiles.
//		Tiles().OnDraw(e);
//
//		// Draw all objects.
//		Objects().OnDraw(e);
//
//		// Draw all foregrounds.
//		Backgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), current_view, true));
//
//	}
//
//	void Room::_initializeAllManagers() {
//
//		if (!Local().IsRegistered<BACKGROUND_MANAGER>()) {
//			auto ptr = make_manager<BackgroundManager>();
//			Local().Register<BACKGROUND_MANAGER>(ptr);
//		}
//
//		if (!Local().IsRegistered<VIEW_MANAGER>()) {
//			auto ptr = make_manager<ViewManager>();
//			Local().Register<VIEW_MANAGER>(ptr);
//		}
//
//		if (!Local().IsRegistered<COLLISION_MANAGER>()) {
//			auto ptr = make_manager<CollisionManager>(std::make_unique<SpatialPartitioningGrid>(32, 32));
//			Local().Register<COLLISION_MANAGER>(ptr);
//		}
//
//		if (!Local().IsRegistered<OBJECT_MANAGER>()) {
//			auto ptr = make_manager<object_manager_type>();
//			Local().Register<OBJECT_MANAGER>(ptr);
//		}
//
//		if (!Local().IsRegistered<PHYSICS_MANAGER>()) {
//			auto ptr = make_manager<physics_manager_type>();
//			Local().Register<PHYSICS_MANAGER>(ptr);
//		}
//
//		if (!Local().IsRegistered<TILE_MANAGER>()) {
//			auto ptr = make_manager<tile_manager_type>(Size(), SizeI(32, 32));
//			Local().Register<TILE_MANAGER>(ptr);
//		}
//
//	}
//
//}