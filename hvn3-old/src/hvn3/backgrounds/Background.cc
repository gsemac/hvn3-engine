#include "hvn3/backgrounds/background.h"

namespace hvn3::backgrounds {

	Background::Background(const Graphics::Bitmap& bitmap) :
		_bitmap(bitmap),
		_offset(0.0f, 0.0f),
		_scale(1.0f, 1.0f) {

		_foreground = false;
		_tile_h = false;
		_tile_v = false;
		_visible = true;
		_fixed = false;

	}

	unsigned int Background::Width() const {

		return _bitmap.Width();

	}
	unsigned int Background::Height() const {

		return _bitmap.Height();

	}
	const Graphics::Bitmap& Background::Bitmap() const {

		return _bitmap;

	}

	const PointF& Background::Offset() const {

		return _offset;

	}
	void Background::SetOffset(const PointF& offset) {

		SetOffset(offset.X(), offset.Y());

	}
	void Background::SetOffset(float x_offset, float y_offset) {

		_offset.SetX(x_offset);
		_offset.SetY(y_offset);

	}
	const Scale& Background::Scale() const {

		return _scale;

	}
	void Background::SetScale(const hvn3::Scale& scale) {

		_scale = scale;

	}
	void Background::SetScale(float x_scale, float y_scale) {

		SetScale(hvn3::Scale(x_scale, y_scale));

	}
	const Vector2d& Background::Velocity() const {

		return _velocity;

	}
	void Background::SetVelocity(const Vector2d& velocity) {

		_velocity = velocity;

	}
	bool Background::IsForeground() const {

		return _foreground;

	}
	void Background::SetForeground(bool value) {

		_foreground = value;

	}
	bool Background::IsTiledHorizontally() const {

		return _tile_h;

	}
	void Background::SetTiledHorizontally(bool value) {

		_tile_h = value;

	}
	bool Background::IsTiledVertically() const {

		return _tile_v;

	}
	void Background::SetTiledVertically(bool value) {

		_tile_v = value;

	}
	bool Background::Visible() const {

		return _visible;

	}
	void Background::SetVisible(bool visible) {

		_visible = visible;

	}
	bool Background::Fixed() const {

		return _fixed;

	}
	void Background::SetFixed(bool fixed) {

		_fixed = fixed;

	}

}