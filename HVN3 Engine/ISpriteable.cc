#include "ISpriteable.h"
#include "DrawEventArgs.h"

namespace hvn3 {

	// Public methods

	ISpriteable::ISpriteable() :
		ISpriteable(nullptr) {
	}
	ISpriteable::ISpriteable(ResourceHandle<hvn3::Sprite> sprite) :
		_image_scale(1.0f),
		_sprite(sprite) {

		_image_index = 0;
		_image_speed = 1.0f;
		_image_alpha = 1.0f;
		_image_angle = 0.0f;
		_image_blend = Color::FromArgbf(_image_alpha, _image_alpha, _image_alpha, _image_alpha);

		_image_index_timer = 0.0f;

	}

	float ISpriteable::ImageAlpha() const {

		return _image_alpha;

	}
	void ISpriteable::SetImageAlpha(float value) {

		_image_alpha = value;

	}

	int ISpriteable::ImageIndex() const {

		return _image_index;

	}
	void ISpriteable::SetImageIndex(float value) {

		_image_index = value;

	}

	float ISpriteable::ImageSpeed() const {

		return _image_speed;

	}
	void ISpriteable::SetImageSpeed(float value) {

		_image_speed = value;

	}

	const Scale& ISpriteable::ImageScale() const {

		return _image_scale;

	}
	void ISpriteable::SetImageScale(const Scale& value) {

		_image_scale = value;

	}

	float ISpriteable::ImageAngle() const {

		return _image_angle;

	}
	void ISpriteable::SetImageAngle(float value) {

		_image_angle = value;

	}

	const Color& ISpriteable::ImageBlend() const {

		return _image_blend;

	}
	void ISpriteable::SetImageBlend(const Color& value) {

		_image_blend = value;

	}

	ResourceHandle<hvn3::Sprite> ISpriteable::Sprite() {

		return _sprite;

	}
	void ISpriteable::SetSprite(ResourceHandle<hvn3::Sprite> sprite) {

		_sprite = sprite;

	}

	void ISpriteable::OnUpdate(UpdateEventArgs& e) {

		IncrementImageIndex();

	}
	void ISpriteable::OnDraw(DrawEventArgs& e) {

		// Draw sprite (if it exists).
		if (Sprite()) {

			e.Graphics().DrawSprite(
				X(),
				Y(),
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

	void ISpriteable::IncrementImageIndex() {

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