#pragma once
#include <memory>
#include "Sprite.h"
#include "Color.h"

namespace hvn3 {

	class ISpriteable {

	public:
		ISpriteable();

		float SpriteIndex();
		float ImageAlpha();
		void SetImageAlpha(float value);
		int ImageIndex();
		void SetImageIndex(float value);
		float ImageSpeed();
		void SetImageSpeed(float value);
		float ImageXScale();
		void SetImageXScale(float value);
		float ImageYScale();
		void SetImageYScale(float value);
		float ImageAngle();
		void SetImageAngle(float value);
		Color ImageBlend();
		void SetImageBlend(const Color& value);

		std::shared_ptr<hvn3::Sprite> Sprite();
		void SetSprite(std::shared_ptr<hvn3::Sprite> sprite);

	private:
		std::shared_ptr<hvn3::Sprite> __sprite;
		int __sprite_index, __image_index;
		float __image_speed, __image_xscale, __image_yscale, __image_angle;
		float __image_alpha;
		Color __image_blend;

	};

}