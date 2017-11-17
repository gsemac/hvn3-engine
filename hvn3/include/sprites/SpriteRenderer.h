#pragma once
#include "assets/ResourceCollection.h"
#include "sprites/Sprite.h"
#include "utility/Scale.h"
#include "core/DrawEventArgs.h"

namespace hvn3 {
	namespace Graphics {

		class SpriteRenderer {

		public:
			SpriteRenderer();
			SpriteRenderer(ResourceHandle<Sprite> sprite);

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

			ResourceHandle<Sprite> Sprite();
			const ResourceHandle<hvn3::Sprite>& Sprite() const;
			void SetSprite(ResourceHandle<hvn3::Sprite> sprite);

			void DrawSprite(DrawEventArgs& e, const PointF& position) const;
			void UpdateAnimation();
			void UpdateAnimation(float delta);

		private:
			ResourceHandle<hvn3::Sprite> _sprite;
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