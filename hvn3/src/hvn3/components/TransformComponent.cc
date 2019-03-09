#include "hvn3/components/TransformComponent.h"

namespace hvn3 {

	float TransformComponent::X() const {

		return _position.x;

	}
	float TransformComponent::Y() const {

		return _position.y;

	}
	void TransformComponent::SetX(float value) {

		_position.x = value;

	}
	void TransformComponent::SetY(float value) {

		_position.y = value;

	}
	const PointF& TransformComponent::Position() const {

		return _position;

	}
	void TransformComponent::SetPosition(const PointF& value) {

		_position = value;

	}
	void TransformComponent::SetPosition(float x, float y) {

		_position = PointF(x, y);

	}

}