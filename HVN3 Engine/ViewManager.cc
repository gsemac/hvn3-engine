#include "ViewManager.h"
#include "RoomBase.h"

namespace hvn3 {

	size_t ViewManager::AddView(const View & view) {

		_views.push_back(view);

		return ViewCount() - 1;

	}
	void ViewManager::RemoveView(size_t index) {

		_views.erase(_views.begin() + index);

	}
	View& ViewManager::ViewAt(size_t index) {

		return _views[index];

	}
	const View& ViewManager::ViewAt(size_t index) const {

		return _views[index];

	}
	size_t ViewManager::ViewCount() const {

		return _views.size();

	}
	void ViewManager::Clear() {

		_views.clear();

	}
	void ViewManager::Update(ViewUpdateEventArgs & e) {

		for (int i = ViewCount() - 1; i >= 0; --i) {

			// Initialize variables.	
			hvn3::View& view = ViewAt(i);
			Object* obj = view.GetFollowing();

			// If the View isn't following an Object, or is disabled, there's nothing to do.
			if (!obj || !view.Enabled())
				continue;

			// Calculate the distance of the Object from the center of the view (to compare with borders).
			float diff_x = (view.ViewX() + view.Region().Width() / 2.0f) - obj->X();
			float diff_y = (view.ViewY() + view.Region().Height() / 2.0f) - obj->Y();

			// Check for overlap in view horizonal view border.
			if ((std::abs)(diff_x) > (view.Region().Width() / 2.0f - view.HorizontalBorder())) {

				// Calculate the amount that the view has to shift by.
				float diff = (view.HorizontalBorder() - ((view.Region().Width() / 2.0f) - (std::abs)(diff_x))) * Signum(diff_x);

				// Make sure the View doesn't shift outside of the room boundaries.
				diff = Clamp(diff, -(e.RoomSize().Width() - view.Region().Width() - view.Position().X()), view.Position().X());

				// Adjust View position.
				view.SetPosition(view.ViewX() - diff, view.ViewY());

			}

			// Check for overlap in view horizonal view border.
			if ((std::abs)(diff_y) > (view.Region().Height() / 2.0f - view.VerticalBorder())) {

				// Calculate the amount that the view has to shift by.
				float diff = (view.VerticalBorder() - ((view.Region().Height() / 2.0f) - (std::abs)(diff_y))) * Signum(diff_y);

				// Make sure the View doesn't shift outside of the room boundaries.
				diff = Clamp(diff, -(e.RoomSize().Height() - view.Region().Height() - view.Position().Y()), view.Position().Y());

				// Adjust View/mouse position.
				view.SetPosition(view.ViewX(), view.ViewY() - diff);

			}

		}

	}

}