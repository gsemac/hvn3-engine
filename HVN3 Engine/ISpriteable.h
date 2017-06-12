#pragma once
#include <memory>
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ResourceCollection.h"
#include "Sprite.h"
#include "Scale.h"
#include "Color.h"

namespace hvn3 {

	class ISpriteable : virtual public IUpdatable, virtual public IDrawable, virtual public IPositionable2d<float> {

	public:
		ISpriteable();
		ISpriteable(ResourceHandle<Sprite> sprite);

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

		void OnUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

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