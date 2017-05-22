#include "BackgroundProperties.h"

namespace hvn3 {

	BackgroundProperties::BackgroundProperties() :
		_offset(0.0f, 0.0f),
		_scale(1.0f, 1.0f),
		_velocity(0.0f, 0.0f) {

		_foreground = false;
		_tile_h = false;
		_tile_v = false;
		_visible = true;
		_fixed = false;

	}

	const Point& BackgroundProperties::Offset() const {

		return _offset;

	}
	void BackgroundProperties::SetOffset(float x_offset, float y_offset) {

		_offset.SetXY(x_offset, y_offset);

	}

	const hvn3::Scale& BackgroundProperties::Scale() const {

		return _scale;

	}
	void BackgroundProperties::SetScale(float x_scale, float y_scale) {

		_scale = hvn3::Scale(x_scale, y_scale);

	}

	const Vector2d& BackgroundProperties::Velocity() const {

		return _velocity;

	}
	void BackgroundProperties::SetVelocity(const Vector2d& velocity) {

		_velocity = velocity;

	}

	bool BackgroundProperties::IsForeground() const {

		return _foreground;

	}
	void BackgroundProperties::SetForeground(bool is_foreground) {

		_foreground = is_foreground;

	}

	bool BackgroundProperties::IsTiledHorizontally() const {

		return _tile_h;

	}
	void BackgroundProperties::SetTiledHorizontally(bool tile_horizontally) {

		_tile_h = tile_horizontally;

	}

	bool BackgroundProperties::IsTiledVertically() const {

		return _tile_v;

	}
	void BackgroundProperties::SetTiledVertically(bool tile_vertically) {

		_tile_v = tile_vertically;

	}

	bool BackgroundProperties::Visible() const {

		return _visible;

	}
	void BackgroundProperties::SetVisible(bool visible) {

		_visible = visible;

	}

	bool BackgroundProperties::Fixed() const {

		return _fixed;

	}
	void BackgroundProperties::SetFixed(bool fixed) {

		_fixed = fixed;

	}

}