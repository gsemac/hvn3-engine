#include "hvn3/backgrounds/BackgroundManager.h"
#include "hvn3/core/CoreTypeDefs.h"
#include "hvn3/views/View.h"
#include <cassert>

namespace hvn3 {

	size_t BackgroundManager::Add(const Background& background) {

		_backgrounds.push_back(background);

		return Count() - 1;

	}
	size_t BackgroundManager::Add(const Background& background, bool foreground) {

		Background bg(background);
		bg.SetForeground(true);

		return Add(bg);

	}
	void BackgroundManager::Remove(size_t index) {

		if (index >= _backgrounds.size())
			return;

		_backgrounds.erase(_backgrounds.begin() + index);

	}
	const Background& BackgroundManager::At(size_t index) const {

		assert(index >= 0);
		assert(index < _backgrounds.size());

		return _backgrounds[index];

	}
	size_t BackgroundManager::Count() const {

		return _backgrounds.size();

	}
	void BackgroundManager::Clear() {

		_backgrounds.clear();

	}
	void BackgroundManager::ForEach(const std::function<bool(Background&)>& func) {

		for (auto i = _backgrounds.begin(); i != _backgrounds.end(); ++i)
			if (func(*i) == BREAK)
				return;

	}
	void BackgroundManager::ForEach(const std::function<bool(const Background&)>& func) const {

		for (auto i = _backgrounds.begin(); i != _backgrounds.end(); ++i)
			if (func(*i) == BREAK)
				return;

	}
	void BackgroundManager::Update(UpdateEventArgs& e) {

		// Update the offset of all moving backgrounds.
		for (size_t i = 0; i < _backgrounds.size(); ++i)
			if (_backgrounds[i].Velocity().Length() != 0.0f)
				_backgrounds[i].SetOffset(_backgrounds[i].Offset().X() + _backgrounds[i].Velocity().X(), _backgrounds[i].Offset().Y() + _backgrounds[i].Velocity().Y());

	}
	void BackgroundManager::Draw(BackgroundDrawEventArgs& e) {

		bool held;
		if (held = Count() > 1 && !e.Graphics().BitmapDrawingHeld())
			e.Graphics().HoldBitmapDrawing(true);

		for (size_t i = 0; i < _backgrounds.size(); ++i) {
			if ((_backgrounds[i].IsForeground() != e.DrawForegrounds()) || !_backgrounds[i].Visible())
				continue;
			_drawBackground(e, _backgrounds[i]);
		}

		if (held)
			e.Graphics().HoldBitmapDrawing(false);

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

		if (background.IsTiledHorizontally() && background.IsTiledVertically())
			// Draw background tiled horizontally and vertically.
			for (; offset.X() < (scale_x < 0.0f ? e.RoomSize().Width() + width : e.RoomSize().Width()); offset.Offset(width, offset.Y()))
				for (float j = offset.Y(); j < ((scale_y < 0.0f) ? (e.RoomSize().Height() + height) : e.RoomSize().Height()); j += height)
					e.Graphics().DrawBitmap(offset.X(), j, background.Bitmap(), background.Scale().XScale(), background.Scale().YScale());
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

}