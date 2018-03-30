#include "hvn3/rooms/Room.h"
#include "hvn3/objects/ObjectManager.h"

namespace hvn3 {

	Room::Room(RoomId id, const SizeI& size) :
		RoomBase<ObjectManager>(id, size) {

		_rendering_view = 0;
		_restart_pending = false;

	}

	void Room::OnUpdate(UpdateEventArgs& e) {

		// Pending restarts should be processed before anything else, so it doesn't do an extra update when resets are initiated outside of it.
		if (_restart_pending) {
			// Reset the state of the room.
			OnReset();
			// Set-up the room again.
			OnSetUp();
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
		if (_view_manager.ViewCount() > 0) {

			for (size_t i = 0; i < _view_manager.ViewCount(); ++i) {

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

	const View& Room::CurrentView() const {

		if (_view_manager.ViewCount() == 0)
			throw System::NullReferenceException();

		return _view_manager.ViewAt(_rendering_view);

	}
	RectangleF Room::VisiblePort() const {

		// Find the largest viewport, or the size of the room if no views are enabled.
		const View* largest = nullptr;
		for (size_t i = 0; i < _view_manager.ViewCount(); ++i) {
			const View* view = &_view_manager.ViewAt(i);
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

		// Find the largest view, or the size of the room if no views are enabled.
		const View* largest = nullptr;
		for (size_t i = 0; i < _view_manager.ViewCount(); ++i) {
			const View* view = &_view_manager.ViewAt(i);
			if (!view->Enabled())
				continue;
			if (largest == nullptr || view->Region().Size() > largest->Region().Size())
				largest = view;
		}

		if (largest == nullptr)
			return RoomBase::VisibleRegion();

		return largest->Region();

	}
	void Room::SetContext(hvn3::Context context) {
		RoomBase::SetContext(context);
		if (!_physics_manager)
			_physics_manager = std::make_unique<hvn3::Physics::BasicPhysicsManager>(Context());
	}

	System::ManagerBase& Room::GetManagerById(System::ManagerId id) {

		auto iter = _registered_managers.find(id);

		if (iter == _registered_managers.end())
			throw System::NullReferenceException();

		return *iter->second;

	}

	void Room::Restart() {

		_restart_pending = true;

	}



	void Room::OnReset() {

		// Reset base (clears all objects).
		RoomBase::OnReset();

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
		const View* current_view = GetViews().ViewCount() > 0 ? &CurrentView() : nullptr;
		GetBackgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), current_view, false));

		// Draw all objects.
		GetObjects().OnDraw(e);

		// Draw all foregrounds.
		GetBackgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), current_view, true));

	}



	void Room::_addManager(System::ManagerId id, System::ManagerBase* manager) {

		auto iter = _registered_managers.find(id);

		if (iter != _registered_managers.end())
			throw System::ArgumentException("A manager with this ID already exists.");

		_registered_managers[id] = manager;

	}

}