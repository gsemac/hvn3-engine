#include "Room.h"

namespace hvn3 {

	Room::Room(RoomId id, const SizeI& size) :
		RoomBase(id, size),
		_rendering_view(0) {
	}
	Room::~Room() {}

	const View* Room::CurrentView() {

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
	void Room::OnUpdate(UpdateEventArgs& e) {

		// Update room.
		RoomBase::OnUpdate(e);

		// Update views.
		_view_manager.Update(ViewUpdateEventArgs(e.Delta(), hvn3::SizeI(Width(), Height())));

		// Update backgrounds.
		_background_manager.Update(e);

	}
	void Room::OnDraw(DrawEventArgs& e) {

		// Save the current graphics state.
		Drawing::Transform original_tranform(e.Graphics().GetTransform());
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
		Backgrounds().DrawBackgrounds(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), CurrentView()));

		// Draw all objects.
		Objects().OnDraw(e);

		// Draw all foregrounds.
		Backgrounds().DrawForegrounds(BackgroundDrawEventArgs(e.Graphics(), SizeI(Width(), Height()), CurrentView()));

	}

}