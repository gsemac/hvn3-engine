#include "RoomBase.h"
#include "UpdateEventArgs.h"
#include "DrawEventArgs.h"
#include "CollisionManager.h"
#include "CollisionGrid.h"

namespace hvn3 {

	RoomBase::RoomBase(unsigned int width, unsigned int height) :
		RoomBase(width, height, std::unique_ptr<IObjectManager>(new hvn3::ObjectManager(std::unique_ptr<hvn3::ICollisionManager>(new hvn3::CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>(new CollisionGrid(32, 32))))))) {
	}
	RoomBase::RoomBase(unsigned int width, unsigned int height, std::unique_ptr<IObjectManager>& object_manager) :
		ISizeable(width, height),
		_obj_manager(std::move(object_manager)) {

		// Set the default background color.
		_background_color = Color::Silver;

		// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
		_persistent = false;
		_set_up = false;

	}
	RoomBase::~RoomBase() {}
	void RoomBase::OnUpdate(UpdateEventArgs& e) {

		// Update objects.
		_obj_manager->OnUpdate(e);

	}
	void RoomBase::OnDraw(DrawEventArgs& e) {

		// Save the current graphics state.
		Drawing::Transform original_tranform(e.Graphics().GetTransform());
		Rectangle original_clip(e.Graphics().Clip());

		// Render the room state.
		Render(e);

		// Restore the previous graphics state.
		e.Graphics().SetTransform(original_tranform);
		e.Graphics().SetClip(original_clip);

	}
	void RoomBase::OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) {}
	void RoomBase::SetBackgroundColor(const Color& color) {

		_background_color = color;

	}
	const Color& RoomBase::BackgroundColor() const {

		return _background_color;

	}
	IObjectManager* RoomBase::ObjectManager() {

		return _obj_manager.get();

	}
	RoomId RoomBase::Id() const {

		return 0;

	}
	bool RoomBase::Persistent() const {

		return _persistent;

	}
	void RoomBase::SetPersistent(bool value) {

		_persistent = value;

	}
	Rectangle RoomBase::GetVisibleRegion() {

		return Rectangle(0, 0, Width(), Height());

	}

	// Protected methods
	void RoomBase::OnRoomEnter(RoomEnterEventArgs& e) {}
	void RoomBase::OnRoomExit(RoomExitEventArgs& e) {}
	void RoomBase::SetUp() {}
	void RoomBase::Reset() {

		// Reset the object manager.
		_obj_manager->Clear();

		// Reset members to default values.
		_background_color = Color::Black;
		_set_up = false;

	}
	void RoomBase::Render(DrawEventArgs& e) {

		// Clear to background color.
		e.Graphics().Clear(_background_color);

		// Draw all objects.
		_obj_manager->OnDraw(e);

	}

}