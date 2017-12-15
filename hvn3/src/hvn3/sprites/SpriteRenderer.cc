#include "hvn3/sprites/SpriteRenderer.h"

namespace hvn3 {
	namespace Graphics {

		SpriteRenderer::SpriteRenderer(const hvn3::Sprite& sprite) :
			_image_scale(1.0f),
			_sprite(&sprite) {

			_image_index = 0;
			_image_speed = 1.0f;
			_image_alpha = 1.0f;
			_image_angle = 0.0f;
			_image_blend = Color::FromArgbf(1.0f, 1.0f, 1.0f);

			_image_index_timer = 0.0f;

		}

		float SpriteRenderer::ImageAlpha() const {

			return _image_alpha;

		}
		void SpriteRenderer::SetImageAlpha(float value) {

			_image_alpha = value;

		}
		int SpriteRenderer::ImageIndex() const {

			return _image_index;

		}
		void SpriteRenderer::SetImageIndex(int value) {

			_image_index = value;

		}
		float SpriteRenderer::ImageSpeed() const {

			return _image_speed;

		}
		void SpriteRenderer::SetImageSpeed(float value) {

			_image_speed = value;

		}
		const Scale& SpriteRenderer::ImageScale() const {

			return _image_scale;

		}
		void SpriteRenderer::SetImageScale(const Scale& value) {

			_image_scale = value;

		}
		float SpriteRenderer::ImageAngle() const {

			return _image_angle;

		}
		void SpriteRenderer::SetImageAngle(float value) {

			_image_angle = value;

		}
		const Color& SpriteRenderer::ImageBlend() const {

			return _image_blend;

		}
		void SpriteRenderer::SetImageBlend(const Color& value) {

			_image_blend = value;

		}
		void SpriteRenderer::ReflectX() {

			_image_scale = Scale(-_image_scale.XScale(), _image_scale.YScale());

		}
		void SpriteRenderer::ReflectY() {

			_image_scale = Scale(_image_scale.XScale(), -_image_scale.YScale());

		}
		const Sprite& SpriteRenderer::Sprite() const {

			return *_sprite;

		}
		void SpriteRenderer::SetSprite(const hvn3::Sprite& sprite) {

			_sprite = &sprite;

		}

		void SpriteRenderer::DrawSprite(DrawEventArgs& e, const PointF& position) const {

			if (_sprite != nullptr && _sprite->Length() > 0) {

				e.Graphics().DrawSprite(
					position.X(),
					position.Y(),
					Sprite(),
					ImageIndex(),
					ImageScale().XScale(),
					ImageScale().YScale(),
					ImageAngle(),
					Color::FromArgbf(
						ImageBlend().Rf() * ImageAlpha(),
						ImageBlend().Gf() * ImageAlpha(),
						ImageBlend().Bf() * ImageAlpha(),
						ImageAlpha()
						)
					);

			}

		}
		void SpriteRenderer::UpdateAnimation() {

			_incrementImageIndex(_image_speed);

		}
		void SpriteRenderer::UpdateAnimation(float delta) {

			_incrementImageIndex(_image_speed * delta);

		}

		// Private methods
		void SpriteRenderer::_incrementImageIndex(float delta) {

			_image_index_timer += (std::fabs)(delta);
			if (_image_index_timer >= 1.0f) {
				switch (static_cast<int>(Math::Sign(ImageSpeed()))) {
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
}