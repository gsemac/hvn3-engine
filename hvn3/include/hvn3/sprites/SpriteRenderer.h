#pragma once
#include "hvn3/sprites/Sprite.h"
#include "hvn3/utility/Scale.h"
#include "hvn3/core/DrawEventArgs.h"

namespace hvn3 {
	namespace Graphics {

		class SpriteRenderer {

		public:
			SpriteRenderer();

			float ImageAlpha() const;
			void SetImageAlpha(float value);

			int ImageIndex() const;
			void SetImageIndex(int value);

			// The speed of the animation in frames-per-update.
			float ImageSpeed() const;
			// Sets the speed of the animation in frames-per-update.
			void SetImageSpeed(float value);

			const Scale& ImageScale() const;
			void SetImageScale(const Scale& scale);

			float ImageAngle() const;
			void SetImageAngle(float value);

			const Color& ImageBlend() const;
			void SetImageBlend(const Color& value);

			void ReflectX();
			void ReflectY();

			void DrawSprite(Graphics& target, const Sprite& sprite, const PointF& position) const;
			void Step();
			void Step(double delta);

		private:
			int _image_index;
			float _image_speed;
			Scale _image_scale;
			float _image_angle;
			float _image_alpha;
			Color _image_blend;
			float _image_index_timer;

			void _incrementImageIndex(float delta);

		};

	}
}