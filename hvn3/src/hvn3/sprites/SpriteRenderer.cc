#include "hvn3/math/MathUtils.h"
#include "hvn3/sprites/SpriteRenderer.h"
#include <limits>

namespace hvn3 {

	SpriteRenderer::SpriteRenderer() :
		_image_scale(1.0f) {

		_image_index = 0;
		_image_speed = 1.0f;
		_image_alpha = 1.0f;
		_image_angle = 0.0f;
		_image_blend = Color::FromArgbf(1.0f, 1.0f, 1.0f);

		_image_index_timer = 0.0f;

	}
	SpriteRenderer::SpriteRenderer(const Sprite& sprite) :
		SpriteRenderer() {

		_sprite = sprite;

	}

	float SpriteRenderer::Opacity() const {
		return _image_alpha;
	}
	void SpriteRenderer::SetOpacity(float value) {
		_image_alpha = value;
	}
	int SpriteRenderer::AnimationIndex() const {
		return _image_index;
	}
	void SpriteRenderer::SetAnimationIndex(int value) {
		_image_index = value;
	}
	float SpriteRenderer::AnimationSpeed() const {
		return _image_speed;
	}
	void SpriteRenderer::SetAnimationSpeed(float fps) {
		_image_speed = fps;
	}
	const Scale& SpriteRenderer::Scale() const {
		return _image_scale;
	}
	void SpriteRenderer::SetScale(const class Scale& value) {
		_image_scale = value;
	}
	float SpriteRenderer::Angle() const {
		return _image_angle;
	}
	void SpriteRenderer::SetAngle(float value) {
		_image_angle = value;
	}
	const Color& SpriteRenderer::Blend() const {
		return _image_blend;
	}
	void SpriteRenderer::SetIBlend(const Color& value) {
		_image_blend = value;
	}

	void SpriteRenderer::UpdateAnimation() {
		_incrementImageIndex(_image_speed);
	}
	void SpriteRenderer::UpdateAnimation(double delta) {
		_incrementImageIndex(_image_speed * static_cast<float>(delta));
	}

	void SpriteRenderer::DrawSprite(Graphics::Graphics& canvas, const PointF& position) const {

		if (_sprite.Length() <= 0)
			return;

		DrawSprite(canvas, _sprite, position);

	}
	void SpriteRenderer::DrawSprite(Graphics::Graphics& canvas, const Sprite& sprite, const PointF& position) const {

		if (sprite.Length() > 0) {

			canvas.DrawSprite(
				position.X(),
				position.Y(),
				sprite,
				AnimationIndex(),
				Scale().XScale(),
				Scale().YScale(),
				_image_angle,
				Color::FromArgbf(
					Blend().Rf() * Opacity(),
					Blend().Gf() * Opacity(),
					Blend().Bf() * Opacity(),
					Opacity()
				)
			);

		}

	}

	void SpriteRenderer::_incrementImageIndex(float delta) {

		_image_index_timer += (std::abs)(delta);
		int increment_dir = static_cast<int>(Math::Sign(AnimationSpeed()));

		while (_image_index_timer >= 1.0f) {

			switch (increment_dir) {
			case -1:
				if (AnimationIndex() == 0)
					SetAnimationIndex(std::numeric_limits<int>::max());
				else
					SetAnimationIndex(AnimationIndex() - 1);
				break;
			case 1:
				if (AnimationIndex() == std::numeric_limits<int>::max())
					SetAnimationIndex(0);
				else
					SetAnimationIndex(AnimationIndex() + 1);
				break;
			}

			_image_index_timer -= 1.0f;

		}

	}

}