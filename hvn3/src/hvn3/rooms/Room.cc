#include "hvn3/rooms/Room.h"
#include "hvn3/objects/ObjectManager.h"

namespace hvn3 {

	Room::Room(RoomId id, const SizeI& size) :
		RoomBase(id, size) {

		_rendering_view = 0;
		_restart_pending = false;

		_initializeAllManagers();

	}
	Room::Room(RoomId id, int width, int height) :
		Room(id, SizeI(width, height)) {
	}
	Room::Room(const SizeI& size) :
		Room(NULL_ROOM_ID, size) {
	}
	Room::Room(int width, int height) :
		Room(NULL_ROOM_ID, width, height) {
	}
	void Room::OnUpdate(UpdateEventArgs& e) {

		// Pending restarts should be processed before anything else, so it doesn't do an extra update when resets are initiated outside of it.
		if (_restart_pending) {
			// Reset the state of the room.
			OnReset();
			// Set-up the room again.
			OnCreate();
			// Disable the restarting pending flag.
			_restart_pending = false;
		}

		// Update objects (begin).
		GetObjects().OnBeginUpdate(e);

		// Update objects (primary).
		GetObjects().OnUpdate(e);

		// Update collision manager.
		GetCollisions().OnUpdate(e);

		// Update physics manager.
		GetPhysics().OnUpdate(e);

		// Update objects (end).
		GetObjects().OnEndUpdate(e);

		// Update views.
		_view_manager.Update(ViewUpdateEventArgs(e.Delta(), hvn3::SizeI(Width(), Height())));

		// Update backgrounds.
		_background_manager.Update(e);

	}
	void Room::OnDraw(DrawEventArgs& e) {

		// Save the current graphics state.
		Graphics::Transform original_tranform(e.Graphics().GetTransform());
		RectangleF original_clip(e.Graphics().Clip());

		// Each View needs to be drawn separately.
		if (_view_manager.Count() > 0) {

			for (size_t i = 0; i < _view_manager.Count(); ++i) {

				// Set current view index.
				_rendering_view = i;

				// If the current view isn't enabled, we don't need to draw it.
				if (!CurrentView().Enabled())
					continue;

				// Set the clipping region according to the view port.
				PointF p1 = CurrentView().Port().TopLeft();
				PointF p2 = CurrentView().Port().BottomRight();
				original_tranform.TransformPoint(p1);
				original_tranform.TransformPoint(p2);
				RectangleF clip(p1, p2);
				e.Graphics().SetClip(clip);

				// Set transform according to view state.		
				Graphics::Transform transform = CurrentView().GetTransform();
				transform.Compose(original_tranform);
				e.Graphics().SetTransform(transform);

				// Render room contents.
				OnRender(e);

			}

			// Reset current view to 0.
			_rendering_view = 0;

		}
		else
			// Simply render the room without any consideration for views.
			OnRender(e);

		// Restore the previous graphics state.
		e.Graphics().SetTransform(original_tranform);
		e.Graphics().SetClip(original_clip);

	}
	IObjectManager& Room::GetObjects() {
		return *_object_manager;
	}
	IBackgroundManager& Room::GetBackgrounds() {
		return _background_manager;
	}
	IViewManager& Room::GetViews() {

		return _view_manager;

	}
	Room::collision_manager_type& Room::GetCollisions() {

		return _collision_manager;

	}
	Physics::IPhysicsManager& Room::GetPhysics() {
		return *_physics_manager;
	}
	TileManager& Room::GetTiles() {
		return *_tile_manager;
	}
	const IObjectManager& Room::Objects() const {
		return *_object_manager;
	}
	const IBackgroundManager& Room::Backgrounds() const {
		return _background_manager;
	}
	const IViewManager& Room::Views() const {

		return _view_manager;

	}
	const Room::collision_manager_type& Room::Collisions() const {

		return _collision_manager;

	}
	const Physics::IPhysicsManager& Room::Physics() const {
		return *_physics_manager;
	}
	const TileManager& Room::Tiles() const {
		return *_tile_manager;
	}
	void Room::SetBackground(const Background& value) {
		GetBackgrounds().Clear();
		GetBackgrounds().Add(value);
	}
	const View& Room::CurrentView() const {

		if (_view_manager.Count() == 0)
			throw System::NullReferenceException();

		return _view_manager.At(_rendering_view);

	}
	RectangleF Room::VisiblePort() const {

		// Find the largest viewport, or the size of the room if no views are enabled.
		const View* largest = nullptr;
		for (size_t i = 0; i < _view_manager.Count(); ++i) {
			const View* view = &_view_manager.At(i);
			if (!view->Enabled())
				continue;
			if (largest == nullptr || view->Port().Size() > largest->Port().Size())
				largest = view;
		}

		if (largest == nullptr)
			return RoomBase::VisiblePort();

		return largest->Port();

	}
	RectangleF Room::VisibleRegion() const {

		// Return the visible region given by the current view (if views are enabled).
		if (_view_manager.Count() > 0)
			return CurrentView().Region();

		// Otherwise, return a region corresponding to the entire room size.
		return RoomBase::VisibleRegion();

	}

	void Room::OnContextChanged(ContextChangedEventArgs& e) {
		RoomBase::OnContextChanged(e);

		_updateContextForAllManagers();

	}
	void Room::Restart() {

		_restart_pending = true;

	}
	void Room::OnReset() {
		RoomBase::OnReset();

		// Reset base (clears all objects).
		_object_manager->Clear();

		// Clear the physics manager first, as it may hold references to the collision manager.
		_physics_manager->Clear();

		// Clear all bodies from the collision manager.
		_collision_manager.Clear();

		// Reset the view manager.
		_view_manager.Clear();
		_rendering_view = 0;

		// Reset the background manager.
		_background_manager.Clear();

	}
	void Room::OnRender(DrawEventArgs& e) {

		// Clear to background color.
		e.Graphics().Clear(BackgroundColor());

		// Draw all backgrounds.
		const View* current_view = GetViews().Count() > 0 ? &CurrentView() : nullptr;
		GetBackgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), current_view, false));

		// Draw all objects.
		GetObjects().OnDraw(e);

		// Draw all foregrounds.
		GetBackgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), current_view, true));

	}

	void Room::_initializeAllManagers() {

		if (!_object_manager)
			_object_manager = std::make_unique<object_manager_type>();

		if (!_physics_manager)
			_physics_manager = std::make_unique<physics_manager_type>();

		if (!_tile_manager)
			_tile_manager = std::make_unique<tile_manager_type>(Size(), SizeI(32, 32));

	}
	void Room::_updateContextForAllManagers() {

		hvn3::Context ctx = Context();

		if (_object_manager)
			ctx.GetContextProvider().ProvideContext(*_object_manager);

		if(_physics_manager)
			ctx.GetContextProvider().ProvideContext(*_physics_manager);

	}

}