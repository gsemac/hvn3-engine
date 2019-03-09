#pragma once

#include "hvn3/ecs/IComponent.h"
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	class TransformComponent :
		public ecs::IComponent {

	public:
		float X() const;
		float Y() const;
		void SetX(float value);
		void SetY(float value);
		const PointF& Position() const;
		void SetPosition(const PointF& value);
		void SetPosition(float x, float y);

	private:
		PointF _position;

	};

}