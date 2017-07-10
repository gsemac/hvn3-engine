#pragma once
#include "ResourceCollection.h"
#include "Sprite.h"
#include "Scale.h"
#include "DrawEventArgs.h"

namespace hvn3 {
	namespace Drawing {

		class SpriteRenderer {

		public:
			SpriteRenderer();
			SpriteRenderer(ResourceHandle<Sprite> sprite);

			float ImageAlpha() const;
			void SetImageAlpha(float value);

			int ImageIndex() const;
			void SetImageIndex(int value);

			float ImageSpeed() const;
			void SetImageSpeed(float value);

			const Scale& ImageScale() const;
			void SetImageScale(const Scale& scale);

			float ImageAngle() const;
			void SetImageAngle(float value);

			const Color& ImageBlend() const;
			void SetImageBlend(const Color& value);

			ResourceHandle<Sprite> Sprite();
			const ResourceHandle<hvn3::Sprite>& Sprite() const;
			void SetSprite(ResourceHandle<hvn3::Sprite> sprite);

			void DrawSprite(const PointF& position, DrawEventArgs& e) const;
			void UpdateAnimation();

		private:
			ResourceHandle<hvn3::Sprite> _sprite;
			int _image_index;
			float _image_speed;
			Scale _image_scale;
			float _image_angle;
			float _image_alpha;
			Color _image_blend;
			float _image_index_timer;

			void IncrementImageIndex();

		};

	}
}