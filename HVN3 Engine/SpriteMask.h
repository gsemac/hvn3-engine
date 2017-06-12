#pragma once
#include <vector>
#include "Geometry.h"
#include "Sprite.h"

namespace hvn3 {

	class SpriteMask {

	private:
		float __degrees;
		std::vector<Rectangle<float>> __strips;
		void GenerateMask(Sprite* sprite);

	public:
		SpriteMask();
		SpriteMask(Sprite* sprite);

		float Angle();
		std::vector<Rectangle<float>>& Strips();

		void Rotate(const Point2d<float>& origin, float degrees, bool relative = true);
		void Translate(float x_offset, float y_offset);
		void Scale(float x_scale, float y_scale);
		void Move(float x, float y);

		bool Intersects(const Circle<float>& other) const;
		bool Intersects(const Rectangle<float>& other) const;
		bool Intersects(const Line<float>& other) const;

		Rectangle<float>& operator[](const size_t& strip);

	};

}