#pragma once
#include <memory>
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ResourceCollection.h"
#include "Sprite.h"
#include "Scale.h"
#include "Color.h"

namespace hvn3 {

	class SpriteableBase {

	public:
		SpriteableBase();
		SpriteableBase(ResourceHandle<Sprite> sprite);

		float ImageAlpha() const;
		void SetImageAlpha(float value);

		int ImageIndex() const;
		void SetImageIndex(float value);

		float ImageSpeed() const;
		void SetImageSpeed(float value);

		const Scale& ImageScale() const;
		void SetImageScale(const Scale& scale);

		float ImageAngle() const;
		void SetImageAngle(float value);

		const Color& ImageBlend() const;
		void SetImageBlend(const Color& value);

		ResourceHandle<Sprite> Sprite();
		void SetSprite(ResourceHandle<hvn3::Sprite> sprite);

	protected:
		void UpdateAnimation(UpdateEventArgs& e);
		void DrawSprite(DrawEventArgs& e, const PointF& position);

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