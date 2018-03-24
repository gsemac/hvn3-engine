#include "hvn3/math/MathUtils.h"
#include "hvn3/sprites/SpriteRenderer.h"
#include <limits>

namespace hvn3 {
	namespace Graphics {

		SpriteRenderer::SpriteRenderer() :
			_image_scale(1.0f) {

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

		int SpriteRenderer::ImageIndex() const {

			return _image_index;

		}

		float SpriteRenderer::ImageSpeed() const {

			return _image_speed;

		}
		const Scale& SpriteRenderer::ImageScale() const {

			return _image_scale;

		}
		float SpriteRenderer::ImageAngle() const {

			return Math::RadiansToDegrees(_image_angle);

		}
		const Color& SpriteRenderer::ImageBlend() const {

			return _image_blend;

		}

		void SpriteRenderer::SetImageAlpha(float value) {

			_image_alpha = value;

		}
		void SpriteRenderer::SetImageIndex(int value) {

			_image_index = value;

		}
		void SpriteRenderer::SetImageSpeed(float value) {

			_image_speed = value;

		}
		void SpriteRenderer::SetImageScale(const Scale& value) {

			_image_scale = value;

		}
		void SpriteRenderer::SetImageAngle(float value) {

			_image_angle = Math::DegreesToRadians(value);

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

		void SpriteRenderer::DrawSprite(Graphics& target, const Sprite& sprite, const PointF& position) const {

			if (sprite.Length() > 0) {

				target.DrawSprite(
					position.X(),
					position.Y(),
					sprite,
					ImageIndex(),
					ImageScale().XScale(),
					ImageScale().YScale(),
					_image_angle, // Make sure that the angle passed in is in radians.
					Color::FromArgbf(
						ImageBlend().Rf() * ImageAlpha(),
						ImageBlend().Gf() * ImageAlpha(),
						ImageBlend().Bf() * ImageAlpha(),
						ImageAlpha()
					)
				);

			}

		}
		void SpriteRenderer::Step() {

			_incrementImageIndex(_image_speed);

		}
		void SpriteRenderer::Step(double delta) {

			_incrementImageIndex(_image_speed * static_cast<float>(delta));

		}



		void SpriteRenderer::_incrementImageIndex(float delta) {

			_image_index_timer += (std::abs)(delta);
			int increment_dir = static_cast<int>(Math::Sign(ImageSpeed()));

			while(_image_index_timer >= 1.0f) {

				switch (increment_dir) {
				case -1:
					if (ImageIndex() == 0)
						SetImageIndex(std::numeric_limits<int>::max());
					else
						SetImageIndex(ImageIndex() - 1);
					break;
				case 1:
					if (ImageIndex() == std::numeric_limits<int>::max())
						SetImageIndex(0);
					else
						SetImageIndex(ImageIndex() + 1);
					break;
				}

				_image_index_timer -= 1.0f;

			}

		}

	}
}