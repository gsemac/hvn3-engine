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
		Objects().OnBeginUpdate(e);

		// Update objects (primary).
		Objects().OnUpdate(e);

		// Update collision manager.
		Collisions().OnUpdate(e);

		// Update physics manager.
		Physics().OnUpdate(e);

		// Update objects (end).
		Objects().OnEndUpdate(e);

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
				if (!CurrentView()->Enabled())
					continue;

				// Set the clipping region according to the view port.
				PointF p1 = CurrentView()->Port().TopLeft();
				PointF p2 = CurrentView()->Port().BottomRight();
				original_tranform.TransformPoint(p1);
				original_tranform.TransformPoint(p2);
				RectangleF clip(p1, p2);
				e.Graphics().SetClip(clip);

				// Set transform according to view state.		
				Graphics::Transform transform = CurrentView()->GetTransform();
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

	const View* Room::CurrentView() const {

		if (_view_manager.ViewCount() == 0)
			return nullptr;

		return &_view_manager.ViewAt(_rendering_view);

	}

	IBackgroundManager& Room::Backgrounds() {

		return _background_manager;

	}
	IViewManager& Room::Views() {

		return _view_manager;

	}
	Room::collision_manager_type& Room::Collisions() {

		return _collision_manager;

	}
	Physics::IPhysicsManager& Room::Physics() {

		return _physics_manager;

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

		return _physics_manager;

	}




	RectangleF Room::GetVisibleRegion() {

		// Return the rectangle representing the largest view.
		View* largest = nullptr;
		for (size_t i = 0; i < _view_manager.ViewCount(); ++i) {

			View* view = &_view_manager.ViewAt(i);

			if (!view->Enabled())
				continue;

			if (largest == nullptr || view->Port().Size() > largest->Port().Size())
				largest = view;

		}

		// If we couldn't find a largest view, return the rectangle representing the full size of the room.
		if (largest == nullptr)
			return RoomBase::GetVisibleRegion();

		// Otherwise, return the viewport.
		return largest->Port();

	}

	void Room::Restart() {

		_restart_pending = true;

	}

	// Protected methods

	void Room::OnReset() {

		// Reset base (clears all objects).
		RoomBase::OnReset();

		// Clear the physics manager first, as it may hold references to the collision manager.
		_physics_manager.Clear();

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
		Backgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), CurrentView(), false));

		// Draw all objects.
		Objects().OnDraw(e);

		// Draw all foregrounds.
		Backgrounds().Draw(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), CurrentView(), true));

	}

}