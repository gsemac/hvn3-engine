#pragma once
#include "hvn3/sprites/Sprite.h"
#include "hvn3/utility/Scale.h"
#include "hvn3/core/DrawEventArgs.h"

namespace hvn3 {

	class SpriteRenderer {

	public:
		SpriteRenderer();
		SpriteRenderer(const Sprite& sprite);

		float Opacity() const;
		void SetOpacity(float value);
		int AnimationIndex() const;
		void SetAnimationIndex(int value);
		float AnimationSpeed() const;
		void SetAnimationSpeed(float fps);
		const Scale& Scale() const;
		void SetScale(const class Scale& value);
		float Angle() const;
		void SetAngle(float value);
		const Color& Blend() const;
		void SetIBlend(const Color& value);

		void UpdateAnimation();
		void UpdateAnimation(double delta);

		void DrawSprite(Graphics::Graphics& canvas, const PointF& position) const;
		void DrawSprite(Graphics::Graphics& canvas, const Sprite& sprite, const PointF& position) const;

	private:
		Sprite _sprite;
		int _image_index;
		float _image_speed;
		class Scale _image_scale;
		float _image_angle;
		float _image_alpha;
		Color _image_blend;
		float _image_index_timer;

		void _incrementImageIndex(float delta);

	};

}