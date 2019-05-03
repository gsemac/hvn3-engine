#include "hvn3/math/MathUtils.h"
#include "hvn3/views/ViewManager.h"

#include <cassert>

namespace hvn3 {

	// Public methods

	ViewManager::index_type ViewManager::AddView(const View& view) {

		_views.push_back(view);

		return Count() - 1;

	}
	void ViewManager::RemoveView(index_type index) {

		assert(index >= 0);
		assert(index < Count());

		_views.erase(_views.begin() + index);

	}
	View& ViewManager::ViewAt(index_type index) {

		assert(index >= 0);
		assert(index < Count());

		return _views[index];

	}
	const View& ViewManager::ViewAt(index_type index) const {

		assert(index >= 0);
		assert(index < _views.size());

		return _views[index];

	}
	ViewManager::size_type ViewManager::Count() const {
		return _views.size();
	}
	void ViewManager::Clear() {
		_views.clear();
	}

	void ViewManager::OnEvent(UpdateEventArgs& e) {

		for (auto i = _views.rbegin(); i != _views.rend(); ++i) {

			hvn3::View& view = *i;
			const ecs::Entity& target = view.Target();

			// If the view isn't following an entity, or it is disabled, there's nothing to update.
			if (!target || !view.Enabled())
				continue;

			// Calculate the distance of the Object from the center of the view (to compare with borders).
			float diff_x = (view.X() + view.Region().Width() / 2.0f) - obj->X();
			float diff_y = (view.Y() + view.Region().Height() / 2.0f) - obj->Y();

			// Check for overlap in view horizonal view border.
			if ((std::abs)(diff_x) > (view.Region().Width() / 2.0f - view.HorizontalBorder())) {

				// Calculate the amount that the view has to shift by.
				float diff = (view.HorizontalBorder() - ((view.Region().Width() / 2.0f) - (std::abs)(diff_x))) * Math::Signum(diff_x);

				// Make sure the View doesn't shift outside of the room boundaries.
				//diff = Math::Clamp(diff, -(e.RoomSize().Width() - view.Region().Width() - view.Position().X()), view.Position().X());

				// Adjust View position.
				view.SetPosition(view.X() - diff, view.Y());

			}

			// Check for overlap in view horizonal view border.
			if ((std::abs)(diff_y) > (view.Region().Height() / 2.0f - view.VerticalBorder())) {

				// Calculate the amount that the view has to shift by.
				float diff = (view.VerticalBorder() - ((view.Region().Height() / 2.0f) - (std::abs)(diff_y))) * Math::Signum(diff_y);

				// Make sure the View doesn't shift outside of the room boundaries.
				//diff = Math::Clamp(diff, -(e.RoomSize().Height() - view.Region().Height() - view.Position().Y()), view.Position().Y());

				// Adjust View/mouse position.
				view.SetPosition(view.X(), view.Y() - diff);

			}


		}

	}

	void ViewManager::ForEach(const std::function<void(View&)>& func) {

		for (auto i = _views.rbegin(); i != _views.rend(); ++i)
			func(*i);

	}
	void ViewManager::ForEach(const std::function<void(const View&)>& func) const {

		for (auto i = _views.rbegin(); i != _views.rend(); ++i)
			func(*i);

	}

}