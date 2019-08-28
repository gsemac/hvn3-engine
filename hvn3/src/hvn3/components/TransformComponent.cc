#include "hvn3/components/TransformComponent.h"

namespace hvn3 {

	TransformComponent::TransformComponent() :
		x(0.0f),
		y(0.0f) {
	}

	float TransformComponent::X() const {

		return x;

	}
	float TransformComponent::Y() const {

		return y;

	}
	void TransformComponent::SetX(float value) {

		x = value;

	}
	void TransformComponent::SetY(float value) {

		y = value;

	}
	PointF TransformComponent::Position() const {

		return PointF(x, y);

	}
	void TransformComponent::SetPosition(const PointF& value) {

		x = value.x;
		y = value.y;

	}
	void TransformComponent::SetPosition(float x, float y) {

		this->x = x;
		this->y = y;

	}

}