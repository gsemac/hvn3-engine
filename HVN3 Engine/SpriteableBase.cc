#include "SpriteableBase.h"
#include "DrawEventArgs.h"

namespace hvn3 {

	// Public methods

	SpriteableBase::SpriteableBase() :
		SpriteableBase(nullptr) {
	}
	SpriteableBase::SpriteableBase(ResourceHandle<hvn3::Sprite> sprite) :
		_image_scale(1.0f),
		_sprite(sprite) {

		_image_index = 0;
		_image_speed = 1.0f;
		_image_alpha = 1.0f;
		_image_angle = 0.0f;
		_image_blend = Color::FromArgbf(_image_alpha, _image_alpha, _image_alpha, _image_alpha);

		_image_index_timer = 0.0f;

	}

	float SpriteableBase::ImageAlpha() const {

		return _image_alpha;

	}
	void SpriteableBase::SetImageAlpha(float value) {

		_image_alpha = value;

	}

	int SpriteableBase::ImageIndex() const {

		return _image_index;

	}
	void SpriteableBase::SetImageIndex(float value) {

		_image_index = value;

	}

	float SpriteableBase::ImageSpeed() const {

		return _image_speed;

	}
	void SpriteableBase::SetImageSpeed(float value) {

		_image_speed = value;

	}

	const Scale& SpriteableBase::ImageScale() const {

		return _image_scale;

	}
	void SpriteableBase::SetImageScale(const Scale& value) {

		_image_scale = value;

	}

	float SpriteableBase::ImageAngle() const {

		return _image_angle;

	}
	void SpriteableBase::SetImageAngle(float value) {

		_image_angle = value;

	}

	const Color& SpriteableBase::ImageBlend() const {

		return _image_blend;

	}
	void SpriteableBase::SetImageBlend(const Color& value) {

		_image_blend = value;

	}

	ResourceHandle<hvn3::Sprite> SpriteableBase::Sprite() {

		return _sprite;

	}
	void SpriteableBase::SetSprite(ResourceHandle<hvn3::Sprite> sprite) {

		_sprite = sprite;

	}

	// Protected methods

	void SpriteableBase::UpdateAnimation(UpdateEventArgs& e) {

		IncrementImageIndex();

	}
	void SpriteableBase::DrawSprite(DrawEventArgs& e, const PointF& position) {

		// Draw sprite (if it exists).
		if (Sprite()) {

			e.Graphics().DrawSprite(
				position.X(),
				position.Y(),
				Sprite(),
				ImageIndex(),
				ImageScale().XScale(),
				ImageScale().YScale(),
				ImageAngle(),
				Color::FromArgbf(1.0f, 1.0f, 1.0f, ImageAlpha())
			);

		}

	}

	// Private methods

	void SpriteableBase::IncrementImageIndex() {

		_image_index_timer += (std::fabs)(ImageSpeed());
		if (_image_index_timer >= 1.0f) {
			switch (Sign(ImageSpeed())) {
			case -1:
				if (ImageIndex() == 0)
					SetImageIndex(INT_MAX);
				else
					SetImageIndex(ImageIndex() - 1);
			case 1:
				if (ImageIndex() == INT_MAX)
					SetImageIndex(0);
				else
					SetImageIndex(ImageIndex() + 1);
			}
			_image_index_timer -= 1.0f;
		}

	}

}