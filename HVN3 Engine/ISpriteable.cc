#include <algorithm>
#include "ISpriteable.h"

namespace hvn3 {

	ISpriteable::ISpriteable() {

		__sprite_index = -1;
		__image_index = 0;
		__image_speed = 1.0f;
		__image_alpha = 1.0f;
		__image_xscale = 1.0f;
		__image_yscale = 1.0f;
		__image_angle = 0.0f;
		__image_blend = Color::FromArgbf(__image_alpha, __image_alpha, __image_alpha, __image_alpha);

	}

	float ISpriteable::SpriteIndex() {

		return __sprite_index;

	}
	float ISpriteable::ImageAlpha() {

		return __image_alpha;

	}
	void ISpriteable::SetImageAlpha(float value) {

		__image_alpha = value;

	}
	int ISpriteable::ImageIndex() {

		return __image_index;

	}
	void ISpriteable::SetImageIndex(float value) {

		__image_index = value;

	}
	float ISpriteable::ImageSpeed() {

		return __image_speed;

	}
	void ISpriteable::SetImageSpeed(float value) {

		__image_speed = value;

	}
	float ISpriteable::ImageXScale() {

		return __image_xscale;

	}
	void ISpriteable::SetImageXScale(float value) {

		__image_xscale = value;

	}
	float ISpriteable::ImageYScale() {

		return __image_yscale;

	}
	void ISpriteable::SetImageYScale(float value) {

		__image_yscale = value;

	}
	float ISpriteable::ImageAngle() {

		return __image_angle;

	}
	void ISpriteable::SetImageAngle(float value) {

		__image_angle = value;

	}
	Color ISpriteable::ImageBlend() {

		return __image_blend;

	}
	void ISpriteable::SetImageBlend(const Color& value) {

		__image_blend = value;

	}

	std::shared_ptr<hvn3::Sprite> ISpriteable::Sprite() {

		return __sprite;

	}
	void ISpriteable::SetSprite(std::shared_ptr<hvn3::Sprite> sprite) {

		__sprite_index = -1;
		__sprite = sprite;

	}

}