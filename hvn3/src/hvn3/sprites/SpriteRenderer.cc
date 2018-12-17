#include "hvn3/math/MathUtils.h"
#include "hvn3/sprites/SpriteRenderer.h"

#include <limits>

namespace hvn3 {

	SpriteRenderer::SpriteRenderer() :
		_image_scale(1.0f) {

		_image_index = 0;
		_image_speed = DEFAULT_ANIMATION_SPEED;
		_image_alpha = 1.0f;
		_image_angle = 0.0f;
		_image_blend = Color::FromArgbf(1.0f, 1.0f, 1.0f);

		_image_index_timer = 0.0f;

	}
	SpriteRenderer::SpriteRenderer(const class Sprite& sprite) :
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

		if (_sprite && _sprite.Length() > 0)
			_image_index %= _sprite.Length();

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
	void SpriteRenderer::SetBlend(const Color& value) {
		_image_blend = value;
	}
	const class Sprite& SpriteRenderer::Sprite() const {
		return _sprite;
	}
	void SpriteRenderer::SetSprite(const class Sprite& value) {

		_sprite = value;

		// Calling this method will force the animation index to be within the bounds of the sprite's subimages.
		SetAnimationIndex(_image_index);

	}

	int SpriteRenderer::UpdateAnimation() {
		return _incrementImageIndex(1.0f);
	}
	int SpriteRenderer::UpdateAnimation(double delta) {
		return _incrementImageIndex(_image_speed * static_cast<float>(delta));
	}

	void SpriteRenderer::DrawSprite(Graphics::Graphics& canvas, const PointF& position) const {

		DrawSprite(canvas, position, _sprite);

	}
	void SpriteRenderer::DrawSprite(Graphics::Graphics& canvas, const PointF& position, const class Sprite& sprite) const {

		if (sprite.Length() <= 0)
			return;

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

	int SpriteRenderer::_incrementImageIndex(float delta) {

		_image_index_timer += (std::abs)(delta);

		int image_index_delta = 0;
		int increment_dir = static_cast<int>(Math::Sign(AnimationSpeed()));
		int max_index = _sprite ? _sprite.Length() : std::numeric_limits<int>::max();

		// Return immediately if the sprite has too few subimages to be animated.
		if (max_index <= 1)
			return image_index_delta;

		while (_image_index_timer >= 1.0f) {

			switch (increment_dir) {
			case -1:
				--image_index_delta;
				_image_index = Math::WrappedIncrement(_image_index, 0, max_index, -1);
				break;
			case 1:
				++image_index_delta;
				_image_index = Math::WrappedIncrement(_image_index, 0, max_index, 1);
				break;
			}

			_image_index_timer -= 1.0f;

		}

		return image_index_delta;

	}

}