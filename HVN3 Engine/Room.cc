#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <cassert>
#include <memory>
#include "Room.h"
#include "Background.h"
#include "Object.h"
#include "Graphics.h"
#include "Mouse.h"
#include "DrawEventArgs.h"
#include "Exception.h"
#include "CollisionManager.h"

Room::Room(unsigned int width, unsigned int height) :
	ISizeable(width, height),
	_obj_manager(std::unique_ptr<::ICollisionManager>(new ::CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>(new CollisionGrid(32, 32))))) {

	// Set the default background color to black.
	_background_color = Color::Black;

	// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
	_rendering_view = 0;
	_persistent = false;
	_set_up = false;

}
void Room::Update(UpdateEventArgs& e) {

	// Update objects.
	_obj_manager.Update(e);

	// Update views.
	_view_manager.Update(ViewUpdateEventArgs(e.Delta(), ::Size(Width(), Height())));

}
void Room::Draw(DrawEventArgs& e) {

	// Save the current graphics state.
	Drawing::Transform original_tranform(e.Graphics().GetTransform());
	Rectangle original_clip(e.Graphics().Clip());

	// Each View needs to be drawn separately.
	if (_view_manager.ViewCount() > 0) {

		for (size_t i = 0; i < _view_manager.ViewCount(); ++i) {

			// Set current view index.
			_rendering_view = i;

			// If the current view isn't enabled, we don't need to draw it.
			if (!CurrentView()->Enabled())
				continue;

			// Set the clipping region according to the view port.
			Point p1 = CurrentView()->Port().TopLeft();
			Point p2 = CurrentView()->Port().BottomRight();
			original_tranform.TransformPoint(p1);
			original_tranform.TransformPoint(p2);
			Rectangle clip(p1, p2);
			e.Graphics().SetClip(clip);

			// Set transform according to view state.		
			Drawing::Transform transform = CurrentView()->GetTransform();
			transform.Compose(original_tranform);
			e.Graphics().SetTransform(transform);

			// Render room contents.
			Render(e);

		}

		// Reset current view to 0.
		_rendering_view = 0;

	}
	else
		// Simply render the room without any consideration for views.
		Render(e);

	// Restore the previous graphics state.
	e.Graphics().SetTransform(original_tranform);
	e.Graphics().SetClip(original_clip);

}
void Room::SetBackgroundColor(const Color& color) {

	_background_color = color;

}
const Color& Room::BackgroundColor() const {

	return _background_color;

}
const View* Room::CurrentView() {

	if (_view_manager.ViewCount() == 0)
		return nullptr;

	return &_view_manager.ViewAt(_rendering_view);

}
ObjectManager* Room::ObjectManager() {

	return &_obj_manager;

}
ViewManager* Room::ViewManager() {

	return &_view_manager;

}
::RoomId Room::Id() const {

	return 0;

}
bool Room::Persistent() const {

	return _persistent;

}
void Room::SetPersistent(bool value) {

	_persistent = value;

}

// Protected methods
void Room::OnRoomEnter(RoomEnterEventArgs& e) {}
void Room::OnRoomExit(RoomExitEventArgs& e) {}
void Room::SetUp() {}
void Room::Reset() {

	// Reset the object manager.
	_obj_manager.Clear();

	// Reset the view manager.
	_view_manager.Clear();

	// Reset members to default values.
	_background_color = Color::Black;
	_rendering_view = 0;
	_set_up = false;

}
void Room::Render(DrawEventArgs& e) {

	// Clear to background color.
	e.Graphics().Clear(_background_color);

	// If no views are used, simply draw all of the objects with normal scaling.
	_obj_manager.Draw(e);

}