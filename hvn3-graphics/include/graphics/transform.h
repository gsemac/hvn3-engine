#pragma once

#include "math/point.h"

#include <allegro5/allegro.h>

namespace hvn3::graphics {

	class Transform {

	public:
		Transform();
		Transform(const Transform& other);

		void Translate(float offsetX, float offsetY);
		void Scale(float scaleX, float scaleY);
		void Rotate(float degrees);
		void Rotate(float originX, float originY, float degrees);
		void Rotate(const math::Point2f& origin, float degrees);
		void HorizontalShear(float degrees);
		void HorizontalShear(float originX, float originY, float degrees);
		void HorizontalShear(const math::Point2f& origin, float degrees);
		void VerticalShear(float degrees);
		void VerticalShear(float originX, float originY, float degrees);
		void VerticalShear(const math::Point2f& origin, float degrees);

		void Compose(const Transform& other);
		void Invert();

		void Reset();

		math::Point2f TransformPoint(const math::Point2f& point) const;

		Transform& operator=(const Transform& other);

	private:
		ALLEGRO_TRANSFORM transform;

		ALLEGRO_TRANSFORM* GetUnderlyingData();
		const ALLEGRO_TRANSFORM* GetUnderlyingData() const;

	};

}