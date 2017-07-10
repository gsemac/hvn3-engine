#include "BackgroundManager.h"
#include "View.h"

namespace hvn3 {

	size_t BackgroundManager::BackgroundAdd(ResourceHandle<Background> background) {

		return BackgroundAdd(background, BackgroundProperties());

	}
	size_t BackgroundManager::BackgroundAdd(ResourceHandle<Background> background, bool is_foreground) {

		BackgroundProperties properties;
		properties.SetForeground(true);

		return BackgroundAdd(background, properties);

	}
	size_t BackgroundManager::BackgroundAdd(ResourceHandle<Background> background, BackgroundProperties properties) {

		_backgrounds.push_back(std::make_pair(background, properties));

		return BackgroundCount() - 1;

	}
	void BackgroundManager::BackgroundRemove(size_t index) {

		_backgrounds.erase(_backgrounds.begin() + index);

	}
	const ResourceHandle<Background>& BackgroundManager::BackgroundAt(size_t index) const {

		return _backgrounds[index].first;

	}
	BackgroundProperties& BackgroundManager::PropertiesAt(size_t index) {

		return _backgrounds[index].second;

	}
	size_t BackgroundManager::BackgroundCount() const {

		return _backgrounds.size();

	}
	void BackgroundManager::Clear() {

		_backgrounds.clear();

	}

	void BackgroundManager::Update(UpdateEventArgs& e) {

		// Update Backgrounds.
		for (size_t i = 0; i < _backgrounds.size(); ++i)
			if (_backgrounds[i].second.Velocity().Magnitude() != 0.0f)
				_backgrounds[i].second.SetOffset(
					_backgrounds[i].second.Offset().X() + _backgrounds[i].second.Velocity().X(),
					_backgrounds[i].second.Offset().Y() + _backgrounds[i].second.Velocity().Y()
					);


	}
	void BackgroundManager::DrawBackgrounds(BackgroundDrawEventArgs& e) {
		
		// Draw all backgrounds.
		e.Graphics().HoldBitmapDrawing(BackgroundCount() > 1);
		for (size_t i = 0; i < _backgrounds.size(); ++i)
			if (!_backgrounds[i].second.IsForeground() && _backgrounds[i].second.Visible())
				DrawBackground(e, _backgrounds[i]);
		e.Graphics().HoldBitmapDrawing(BackgroundCount() > 1);

	}
	void BackgroundManager::DrawForegrounds(BackgroundDrawEventArgs& e) {

		// Draw all foregrounds.
		e.Graphics().HoldBitmapDrawing(BackgroundCount() > 1);
		for (size_t i = 0; i < _backgrounds.size(); ++i)
			if (_backgrounds[i].second.IsForeground() && _backgrounds[i].second.Visible())
				DrawBackground(e, _backgrounds[i]);
		e.Graphics().HoldBitmapDrawing(BackgroundCount() > 1);

	}

	// Protected methods

	void BackgroundManager::DrawBackground(BackgroundDrawEventArgs& e, const BackgroundManager::bg_type& background) const {

		// Get references to the background and properties objects.
		const Background& bg = *background.first;
		const BackgroundProperties& p = background.second;

		// Calculate scaled dimensions of the background.
		float scale_x = p.Scale().XScale();
		float scale_y = p.Scale().YScale();
		float width = bg.Width() * Math::Abs(scale_x);
		float height = bg.Height() * Math::Abs(scale_y);

		// If either dimension is 0, don't draw.
		if (Math::IsZero(width, 0.1f) || Math::IsZero(height, 0.1f))
			return;

		// Calculate the offset of the background. If the background is tiled, this is the starting offset.
		PointF offset = background.second.Offset();

		// If the background is fixed, counteract this by the view offset.
		if (p.Fixed() && e.CurrentView())
			offset += e.CurrentView()->Position();

		if (p.IsTiledHorizontally())
			// Subtract the width of the background from the offset until it is "0" or negative.
			while (offset.X() > 0.1f) offset.Offset(-width, offset.Y());

		if (p.IsTiledVertically())
			// Subtract the height of the background from the offset until it is "0" or negative.
			while (offset.Y() > 0.1f) offset.Offset(offset.X(), -height);

		// If the scale is negative, adjust the offset so that the tiles are drawn in the proper location (it will be flipped over axis).
		if (scale_x < 0.0f) offset.Offset(width, offset.Y());
		if (scale_y < 0.0f) offset.Offset(offset.X(), height);

		if (p.IsTiledHorizontally() && p.IsTiledVertically())
			// Draw background tiled horizontally and vertically.
			for (; offset.X() < (scale_x < 0.0f ? e.RoomSize().Width() + width : e.RoomSize().Width()); offset.Offset(width, offset.Y()))
				for (float j = offset.Y(); j < ((scale_y < 0.0f) ? (e.RoomSize().Height() + height) : e.RoomSize().Height()); j += height)
					e.Graphics().DrawBitmap(offset.X(), j, &bg.Bitmap(), p.Scale().XScale(), p.Scale().YScale());
		else if (p.IsTiledHorizontally())
			// Draw background tiled horizontally only.
			for (; offset.X() < (scale_x < 0.0f ? e.RoomSize().Width() + width : e.RoomSize().Width()); offset.Offset(width, offset.Y()))
				e.Graphics().DrawBitmap(offset.X(), offset.Y(), &bg.Bitmap(), p.Scale().XScale(), p.Scale().YScale());
		else if (p.IsTiledVertically())
			// Draw background tiled vertically only.
			for (; offset.Y() < (scale_y < 0.0f ? e.RoomSize().Height() + height : e.RoomSize().Height()); offset.Offset(offset.X(), height))
				e.Graphics().DrawBitmap(offset.X(), offset.Y(), &bg.Bitmap(), p.Scale().XScale(), p.Scale().YScale());
		else
			// Draw background without tiling.
			e.Graphics().DrawBitmap(offset.X(), offset.Y(), &bg.Bitmap(), p.Scale().XScale(), p.Scale().YScale());

	}

}