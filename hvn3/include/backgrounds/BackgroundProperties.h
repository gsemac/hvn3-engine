#pragma once
#include "utility/Point2d.h"
#include "math/Vector2d.h"
#include "utility/Scale.h"

namespace hvn3 {

	class BackgroundProperties {

	public:
		BackgroundProperties();

		const PointF& Offset() const;
		void SetOffset(float x_offset, float y_offset);

		const Scale& Scale() const;
		void SetScale(float x_scale, float y_scale);

		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& velocity);

		bool IsForeground() const;
		void SetForeground(bool is_foreground);

		bool IsTiledHorizontally() const;
		void SetTiledHorizontally(bool tile_horizontally);

		bool IsTiledVertically() const;
		void SetTiledVertically(bool tile_vertically);

		bool Visible() const;
		void SetVisible(bool visible);

		bool Fixed() const;
		void SetFixed(bool fixed);

	private:
		PointF _offset;
		hvn3::Scale _scale;
		Vector2d _velocity;
		bool _foreground;
		bool _tile_h, _tile_v;
		bool _visible;
		bool _fixed;

	};

}