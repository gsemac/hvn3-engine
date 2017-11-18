#pragma once
#include "graphics/Bitmap.h"
#include "math/Vector2d.h"
#include "utility/Point2d.h"
#include "utility/Scale.h"

namespace hvn3 {

	class Background {

	public:
		Background(const Graphics::Bitmap& bitmap);

		// Returns the width of the underlying bitmap.
		unsigned int Width() const;
		// Returns the height of the underlying bitmap.
		unsigned int Height() const;
		// Returns the underlying bitmap.
		const Graphics::Bitmap& Bitmap() const;

		// Returns the background's offet.
		const PointF& Offset() const;
		// Sets the background's offset.
		void SetOffset(const PointF& offset);
		// Sets the background's offset.
		void SetOffset(float x_offset, float y_offset);
		// Returns the background's scaling factor.
		const Scale& Scale() const;
		// Sets the background's scaling factor.
		void SetScale(const hvn3::Scale& scale);
		// Sets the background's scaling factor.
		void SetScale(float x_scale, float y_scale);
		// Returns the background's velocity.
		const Vector2d& Velocity() const;
		// Sets the background's velocity.
		void SetVelocity(const Vector2d& velocity);
		// Returns true if the background is a foreground. Foregrounds are drawn in front of objects and other backgrounds.
		bool IsForeground() const;
		// Sets the background's foreground flag.
		void SetForeground(bool value);
		// Returns true if the background is set to be tiled horizontally when drawn.
		bool IsTiledHorizontally() const;
		// Sets the background's tiled horizontally flag.
		void SetTiledHorizontally(bool value);
		// Returns true if the background is set to be tiled vertically when drawn.
		bool IsTiledVertically() const;
		// Sets the background's tiled vertically flag.
		void SetTiledVertically(bool value);
		// Returns true if the background is visible.
		bool Visible() const;
		// Sets the background's visible flag.
		void SetVisible(bool visible);
		// Returns true if the background is fixed. This keeps the background in the same position relative to the view.
		bool Fixed() const;
		// Sets the background's fixed flag.
		void SetFixed(bool fixed);

	private:
		Graphics::Bitmap _bitmap;
		PointF _offset;
		hvn3::Scale _scale;
		Vector2d _velocity;
		bool _foreground;
		bool _tile_h, _tile_v;
		bool _visible;
		bool _fixed;

	};

}