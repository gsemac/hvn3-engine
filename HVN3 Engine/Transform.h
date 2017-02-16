#pragma once
#include <allegro5/allegro_primitives.h>
#include "Point.h"

namespace Drawing {

	class Transform {

	public:
		Transform();
		Transform(const Transform& other);

		// Resets the transformation back to its default state.
		void Reset();
		void Invert();
		void Translate(float x_offset, float y_offset);
		void Scale(float x_factor, float y_factor);
		void Rotate(float degrees);
		void Rotate(float origin_x, float origin_y, float degrees);
		void Rotate(const Point& origin, float degrees);
		void Compose(const Transform& other);
		void HorizontalShear(float degrees);
		void HorizontalShear(float origin_x, float origin_y, float degrees);
		void HorizontalShear(const Point& origin, float degrees);
		void VerticalShear(float degrees);
		void VerticalShear(float origin_x, float origin_y, float degrees);
		void VerticalShear(const Point& origin, float degrees);
		void TransformPoint(float& x, float& y) const;
		void TransformPoint(Point& point) const;
		const ALLEGRO_TRANSFORM* AlPtr() const;
		Transform& operator=(const Transform& other);

	private:
		ALLEGRO_TRANSFORM __t;

	};

}