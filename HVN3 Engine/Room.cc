#include "Room.h"

namespace hvn3 {

	Room::Room(unsigned int width, unsigned int height) :
		RoomBase(width, height),
		_rendering_view(0) {

	}
	Room::~Room() {}

	const View* Room::CurrentView() {

		if (_view_manager.ViewCount() == 0)
			return nullptr;

		return &_view_manager.ViewAt(_rendering_view);

	}
	IBackgroundManager* Room::BackgroundManager() {

		return &_background_manager;

	}
	IViewManager* Room::ViewManager() {

		return &_view_manager;

	}
	void Room::Update(UpdateEventArgs& e) {

		// Update room.
		RoomBase::Update(e);

		// Update views.
		_view_manager.Update(ViewUpdateEventArgs(e.Delta(), hvn3::Size(Width(), Height())));

		// Update backgrounds.
		_background_manager.Update(e);

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

	// Protected methods

	void Room::Reset() {

		// Reset room.
		RoomBase::Reset();

		// Reset the view manager.
		_view_manager.Clear();
		_rendering_view = 0;

		// Reset the background manager.
		_background_manager.Clear();

	}
	void Room::Render(DrawEventArgs& e) {

		// Clear to background color.
		e.Graphics().Clear(BackgroundColor());

		// Draw all backgrounds.
		BackgroundManager()->DrawBackgrounds(BackgroundDrawEventArgs(e.Graphics(), Size(Width(), Height()), &_view_manager.ViewAt(_rendering_view)));

		// Draw all objects.
		ObjectManager()->Draw(e);

		// Draw all foregrounds.
		BackgroundManager()->DrawForegrounds(BackgroundDrawEventArgs(e.Graphics(), Size(Width(), Height()), &_view_manager.ViewAt(_rendering_view)));

	}

}