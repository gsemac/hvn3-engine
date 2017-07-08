#include "RoomBase.h"
#include "UpdateEventArgs.h"
#include "DrawEventArgs.h"
#include "CollisionManager.h"
#include "CollisionGrid.h"

namespace hvn3 {

	RoomBase::RoomBase(RoomId id, const SizeI& size) :
		IRoom(size) {

		_id = id;

		// Set the default background color.
		_background_color = Color::Silver;

		// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
		_persistent = false;
		_set_up = false;

	}

	void RoomBase::OnUpdate(UpdateEventArgs& e) {

		// Update objects.
		_obj_manager.OnUpdate(e);

	}
	void RoomBase::OnDraw(DrawEventArgs& e) {

		// Save the current graphics state.
		Drawing::Transform original_tranform(e.Graphics().GetTransform());
		RectangleF original_clip(e.Graphics().Clip());

		// Render the room state.
		OnRender(e);

		// Restore the previous graphics state.
		e.Graphics().SetTransform(original_tranform);
		e.Graphics().SetClip(original_clip);

	}
	void RoomBase::OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) {}

	const View* RoomBase::CurrentView() const {

		return nullptr;

	}

	const IObjectManager* RoomBase::Objects() const {

		return &_obj_manager;

	}
	IObjectManager* RoomBase::Objects() {
		
		return &_obj_manager;

	}
	const IBackgroundManager* RoomBase::Backgrounds() const {

		return nullptr;

	}
	IBackgroundManager* RoomBase::Backgrounds() {

		return nullptr;

	}
	const IViewManager* RoomBase::Views() const {

		return nullptr;

	}
	IViewManager* RoomBase::Views() {

		return nullptr;

	}
	const ICollisionManager<Object*>* RoomBase::Collisions() const {

		return nullptr;

	}
	ICollisionManager<Object*>* RoomBase::Collisions() {

		return nullptr;

	}
	const Physics::IPhysicsManager<Object*>* RoomBase::Physics() const {

		return nullptr;

	}
	Physics::IPhysicsManager<Object*>* RoomBase::Physics() {

		return nullptr;

	}

	RoomId RoomBase::Id() const {

		return _id;

	}
	RectangleF RoomBase::GetVisibleRegion() {

		return RectangleF(0, 0, Width(), Height());

	}

	void RoomBase::SetBackgroundColor(const Color& color) {

		_background_color = color;

	}
	const Color& RoomBase::BackgroundColor() const {

		return _background_color;

	}

	bool RoomBase::Persistent() const {

		return _persistent;

	}
	void RoomBase::SetPersistent(bool value) {

		_persistent = value;

	}
	
	// Protected methods
	void RoomBase::OnRoomEnter(RoomEnterEventArgs& e) {}
	void RoomBase::OnRoomExit(RoomExitEventArgs& e) {}
	void RoomBase::OnSetUp() {}
	void RoomBase::OnReset() {

		// Reset the object manager.
		_obj_manager.Clear();

		// Reset members to default values.
		_background_color = Color::Black;
		_set_up = false;

	}
	void RoomBase::OnRender(DrawEventArgs& e) {

		// Clear to background color.
		e.Graphics().Clear(_background_color);

		// Draw all objects.
		_obj_manager.OnDraw(e);

	}

}