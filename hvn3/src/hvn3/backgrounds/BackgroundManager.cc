#include "hvn3/backgrounds/BackgroundManager.h"
#include "hvn3/core/CoreDefs.h"
#include "hvn3/events/IEventManager.h"
#include "hvn3/events/EventListenerRegistry.h"
#include "hvn3/views/View.h"

#include <cassert>

namespace hvn3 {

	// Public methods

	BackgroundManager::BackgroundManager() :
		_background_color(Color::Black) {
	}

	void BackgroundManager::OnStart(StartEventArgs& e) {

		// Subscribe to events.

		if (auto handle = e.Context().Get<IEventManager>())
			handle->GetListenerRegistry().SubscribeAll(this, EventListenerPriority::HIGH_PRIORITY);

	}
	void BackgroundManager::OnEnd(EndEventArgs& e) {

		// Unsubscribe from events.

		if (auto handle = e.Context().Get<IEventManager>())
			handle->GetListenerRegistry().UnsubscribeAll(this);

	}
	void BackgroundManager::OnEvent(DrawEventArgs& e) {

		e.Graphics().Clear(_background_color);

	}
	void BackgroundManager::OnEvent(UpdateEventArgs& e) {

		// Update the offsets of all moving backgrounds.

		for (size_t i = 0; i < _backgrounds.size(); ++i)
			_backgrounds[i].SetOffset(_backgrounds[i].Offset().X() + _backgrounds[i].Velocity().X(), _backgrounds[i].Offset().Y() + _backgrounds[i].Velocity().Y());

	}

	BackgroundManager::index_type BackgroundManager::AddBackground(const Background& background) {

		_backgrounds.push_back(background);

		return Count() - 1;

	}
	BackgroundManager::index_type BackgroundManager::AddBackground(const Background& background, bool foreground) {

		Background bg(background);
		bg.SetForeground(foreground);

		return AddBackground(bg);

	}
	void BackgroundManager::RemoveBackgroundAt(index_type index) {

		assert(index >= 0);
		assert(index < _backgrounds.size());

		if (index >= _backgrounds.size())
			return;

		_backgrounds.erase(_backgrounds.begin() + index);

	}
	const Background& BackgroundManager::BackgroundAt(index_type index) const {

		assert(index >= 0);
		assert(index < _backgrounds.size());

		return _backgrounds[index];

	}

	void BackgroundManager::SetBackgroundColor(const Color& color) {
		_background_color = color;
	}

	BackgroundManager::size_type BackgroundManager::Count() const {
		return _backgrounds.size();
	}
	void BackgroundManager::Clear() {
		_backgrounds.clear();
	}

	/*

	void BackgroundManager::Draw(BackgroundDrawEventArgs& e) {

		bool held;
		if (held = Count() > 1 && !e.Graphics().BitmapDrawingHeld())
			e.Graphics().SetBitmapDrawingHeld(true);

		for (size_t i = 0; i < _backgrounds.size(); ++i) {
			if ((_backgrounds[i].IsForeground() != e.DrawForegrounds()) || !_backgrounds[i].Visible())
				continue;
			_drawBackground(e, _backgrounds[i]);
		}

		if (held)
			e.Graphics().SetBitmapDrawingHeld(false);

	}

	void BackgroundManager::_drawBackground(BackgroundDrawEventArgs& e, const Background& background) const {

		// Calculate scaled dimensions of the background.
		float scale_x = background.Scale().XScale();
		float scale_y = background.Scale().YScale();
		float width = background.Width() * Math::Abs(scale_x);
		float height = background.Height() * Math::Abs(scale_y);

		// If either dimension is 0, don't draw.
		if (Math::IsZero(width, 0.1f) || Math::IsZero(height, 0.1f))
			return;

		// Calculate the offset of the background. If the background is tiled, this is the starting offset.
		PointF offset = background.Offset();

		// If the background is fixed, counteract this by the view offset.
		if (background.Fixed() && e.CurrentView())
			offset += e.CurrentView()->Position();

		if (background.IsTiledHorizontally())
			// Subtract the width of the background from the offset until it is "0" or negative.
			while (offset.X() > 0.1f) offset.Offset(-width, offset.Y());

		if (background.IsTiledVertically())
			// Subtract the height of the background from the offset until it is "0" or negative.
			while (offset.Y() > 0.1f) offset.Offset(offset.X(), -height);

		// If the scale is negative, adjust the offset so that the tiles are drawn in the proper location (it will be flipped over axis).
		if (scale_x < 0.0f) offset.Offset(width, offset.Y());
		if (scale_y < 0.0f) offset.Offset(offset.X(), height);

		if (background.IsTiledHorizontally() && background.IsTiledVertically()) {

			// Draw background tiled horizontally and vertically.
			for (; offset.X() < (scale_x < 0.0f ? e.RoomSize().Width() + width : e.RoomSize().Width()); offset.Offset(width, 0.0f))
				for (float j = offset.Y(); j < ((scale_y < 0.0f) ? (e.RoomSize().Height() + height) : e.RoomSize().Height()); j += height)
					e.Graphics().DrawBitmap(offset.X(), j, background.Bitmap(), background.Scale().XScale(), background.Scale().YScale());

		}
		else if (background.IsTiledHorizontally())
			// Draw background tiled horizontally only.
			for (; offset.X() < (scale_x < 0.0f ? e.RoomSize().Width() + width : e.RoomSize().Width()); offset.Offset(width, offset.Y()))
				e.Graphics().DrawBitmap(offset.X(), offset.Y(), background.Bitmap(), background.Scale().XScale(), background.Scale().YScale());
		else if (background.IsTiledVertically())
			// Draw background tiled vertically only.
			for (; offset.Y() < (scale_y < 0.0f ? e.RoomSize().Height() + height : e.RoomSize().Height()); offset.Offset(offset.X(), height))
				e.Graphics().DrawBitmap(offset.X(), offset.Y(), background.Bitmap(), background.Scale().XScale(), background.Scale().YScale());
		else
			// Draw background without tiling.
			e.Graphics().DrawBitmap(offset.X(), offset.Y(), background.Bitmap(), background.Scale().XScale(), background.Scale().YScale());

	}

	*/

}