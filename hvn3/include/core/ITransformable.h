#pragma once
#include "Point.h"

namespace hvn3 {

	class ITransformable {

	public:
		virtual void Rotate(Point origin, float degrees, bool relative = true) = 0;
		virtual void Translate(float x_offset, float y_offset) = 0;
		virtual void Scale(float x_scale, float y_scale) = 0;
		virtual void Move(float x, float y) = 0;

	};

}