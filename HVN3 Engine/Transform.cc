#include "Transform.h"
#include "Utility.h"

namespace Drawing {

	Transform::Transform() {

		// Initialize the transform.
		Reset();

	}
	Transform::Transform(const Transform& other) {

		al_copy_transform(&__t, &other.__t);

	}
	void Transform::Reset() {

		al_identity_transform(&__t);

	}
	void Transform::Invert() {

		al_invert_transform(&__t);

	}
	void Transform::Translate(float x_offset, float y_offset) {

		al_translate_transform(&__t, x_offset, y_offset);

	}
	void Transform::Scale(float x_factor, float y_factor) {

		al_scale_transform(&__t, x_factor, y_factor);

	}
	void Transform::Scale(const ::Scale& scale) {

		al_scale_transform(&__t, scale.XScale(), scale.YScale());

	}
	void Transform::Rotate(float degrees) {

		al_rotate_transform(&__t, DegreesToRadians(degrees));

	}
	void Transform::Rotate(float origin_x, float origin_y, float degrees) {

		al_translate_transform(&__t, -origin_x, -origin_y);
		al_rotate_transform(&__t, DegreesToRadians(degrees));
		al_translate_transform(&__t, origin_x, origin_y);

	}
	void Transform::Rotate(const Point& origin, float degrees) {

		Rotate(origin.X(), origin.Y(), degrees);

	}
	void Transform::Compose(const Transform& other) {

		al_compose_transform(&__t, other.AlPtr());

	}
	void Transform::HorizontalShear(float degrees) {

		al_horizontal_shear_transform(&__t, DegreesToRadians(degrees));

	}
	void Transform::HorizontalShear(float origin_x, float origin_y, float degrees) {

		al_translate_transform(&__t, -origin_x, -origin_y);
		al_horizontal_shear_transform(&__t, DegreesToRadians(degrees));
		al_translate_transform(&__t, origin_x, origin_y);

	}
	void Transform::HorizontalShear(const Point& origin, float degrees) {

		HorizontalShear(origin.X(), origin.Y(), degrees);

	}
	void Transform::VerticalShear(float degrees) {

		al_vertical_shear_transform(&__t, DegreesToRadians(degrees));

	}
	void Transform::VerticalShear(float origin_x, float origin_y, float degrees) {

		al_translate_transform(&__t, -origin_x, -origin_y);
		al_vertical_shear_transform(&__t, DegreesToRadians(degrees));
		al_translate_transform(&__t, origin_x, origin_y);

	}
	void Transform::VerticalShear(const Point& origin, float degrees) {

		VerticalShear(origin.X(), origin.Y(), degrees);

	}
	void Transform::TransformPoint(float& x, float& y) const {

		al_transform_coordinates(&__t, &x, &y);

	}
	void Transform::TransformPoint(Point& point) const {

		float x = point.X();
		float y = point.Y();
		TransformPoint(x, y);
		point.SetXY(x, y);

	}
	const ALLEGRO_TRANSFORM* Transform::AlPtr() const {

		return &__t;

	}
	Transform& Transform::operator=(const Transform& other) {

		al_identity_transform(&__t);
		al_copy_transform(&__t, &other.__t);

		return *this;

	}

	Point Transform::GetScale() const {

		float scale_x = Sign(__t.m[0][0]) * std::sqrt(std::pow(__t.m[0][0], 2.0f) + std::pow(__t.m[1][0], 2.0f));
		float scale_y = Sign(__t.m[1][1]) * std::sqrt(std::pow(__t.m[0][1], 2.0f) + std::pow(__t.m[1][1], 2.0f));

		return Point(scale_x, scale_y);

	}
	float Transform::GetAngle() const {
		
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j)
				std::cout << __t.m[j][i] << " ";
			std::cout << std::endl;
		}

		return RadiansToDegrees(std::atan2(__t.m[0][1],__t.m[1][1]));

		//return 0;

	}
	Point Transform::GetOffset() const {

		return Point(__t.m[3][0], __t.m[3][1]);

	}

}

