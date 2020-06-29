#include "graphics/transform.h"

#include "math/math.h"

namespace hvn3::graphics {

	// Public members

	Transform::Transform() {

		Reset();

	}
	Transform::Transform(const Transform& other) {

		al_copy_transform(GetUnderlyingData(), other.GetUnderlyingData());

	}

	void Transform::Translate(float offsetX, float offsetY) {

		al_translate_transform(GetUnderlyingData(), offsetX, offsetY);

	}
	void Transform::Scale(float scaleX, float scaleY) {

		al_scale_transform(GetUnderlyingData(), scaleX, scaleY);

	}
	void Transform::Rotate(float degrees) {

		al_rotate_transform(GetUnderlyingData(), math::DegreesToRadians(degrees));

	}
	void Transform::Rotate(float originX, float originY, float degrees) {

		Translate(-originX, -originY);
		Rotate(degrees);
		Translate(originX, originY);

	}
	void Transform::Rotate(const math::Point2f& origin, float degrees) {

		Rotate(origin.X(), origin.Y(), degrees);

	}
	void Transform::HorizontalShear(float degrees) {

		al_horizontal_shear_transform(GetUnderlyingData(), math::DegreesToRadians(degrees));

	}
	void Transform::HorizontalShear(float originX, float originY, float degrees) {

		Translate(-originX, -originY);
		HorizontalShear(degrees);
		Translate(originX, originY);

	}
	void Transform::HorizontalShear(const math::Point2f& origin, float degrees) {

		HorizontalShear(origin.X(), origin.Y(), degrees);

	}
	void Transform::VerticalShear(float degrees) {

		al_vertical_shear_transform(GetUnderlyingData(), math::DegreesToRadians(degrees));

	}
	void Transform::VerticalShear(float originX, float originY, float degrees) {

		Translate(-originX, -originY);
		VerticalShear(degrees);
		Translate(originX, originY);

	}
	void Transform::VerticalShear(const math::Point2f& origin, float degrees) {

		VerticalShear(origin.X(), origin.Y(), degrees);

	}

	void Transform::Compose(const Transform& other) {

		al_compose_transform(GetUnderlyingData(), other.GetUnderlyingData());

	}
	void Transform::Invert() {

		al_invert_transform(GetUnderlyingData());

	}

	void Transform::Reset() {

		al_identity_transform(GetUnderlyingData());

	}

	math::Point2f Transform::TransformPoint(const math::Point2f& point) const {

		math::Point2f transformedPoint = point;

		al_transform_coordinates(GetUnderlyingData(), &transformedPoint.x, &transformedPoint.y);

		return transformedPoint;

	}

	Transform& Transform::operator=(const Transform& other) {

		Reset();

		al_copy_transform(GetUnderlyingData(), other.GetUnderlyingData());

		return *this;

	}

	// Private members

	ALLEGRO_TRANSFORM* Transform::GetUnderlyingData() {

		return &transform;

	}
	const ALLEGRO_TRANSFORM* Transform::GetUnderlyingData() const {

		return &transform;

	}

}