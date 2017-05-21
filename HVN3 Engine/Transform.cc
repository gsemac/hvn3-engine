#include "Transform.h"
#include "Utility.h"
#include <allegro5/allegro.h>

namespace Drawing {

	Transform::Transform() {

		// Initialize the transform.
		Reset();

	}
	Transform::Transform(const Transform& other) {

		al_copy_transform((ALLEGRO_TRANSFORM*)this, (ALLEGRO_TRANSFORM*)&other);

	}
	void Transform::Reset() {

		al_identity_transform((ALLEGRO_TRANSFORM*)this);

	}
	void Transform::Invert() {

		al_invert_transform((ALLEGRO_TRANSFORM*)this);

	}
	void Transform::Translate(float x_offset, float y_offset) {

		al_translate_transform((ALLEGRO_TRANSFORM*)this, x_offset, y_offset);

	}
	void Transform::Scale(float x_factor, float y_factor) {

		al_scale_transform((ALLEGRO_TRANSFORM*)this, x_factor, y_factor);

	}
	void Transform::Scale(const ::Scale& scale) {

		al_scale_transform((ALLEGRO_TRANSFORM*)this, scale.XScale(), scale.YScale());

	}
	void Transform::Rotate(float degrees) {

		al_rotate_transform((ALLEGRO_TRANSFORM*)this, DegreesToRadians(degrees));

	}
	void Transform::Rotate(float origin_x, float origin_y, float degrees) {

		al_translate_transform((ALLEGRO_TRANSFORM*)this, -origin_x, -origin_y);
		al_rotate_transform((ALLEGRO_TRANSFORM*)this, DegreesToRadians(degrees));
		al_translate_transform((ALLEGRO_TRANSFORM*)this, origin_x, origin_y);

	}
	void Transform::Rotate(const Point& origin, float degrees) {

		Rotate(origin.X(), origin.Y(), degrees);

	}
	void Transform::Compose(const Transform& other) {

		al_compose_transform((ALLEGRO_TRANSFORM*)this, (ALLEGRO_TRANSFORM*)&other);

	}
	void Transform::HorizontalShear(float degrees) {

		al_horizontal_shear_transform((ALLEGRO_TRANSFORM*)this, DegreesToRadians(degrees));

	}
	void Transform::HorizontalShear(float origin_x, float origin_y, float degrees) {

		al_translate_transform((ALLEGRO_TRANSFORM*)this, -origin_x, -origin_y);
		al_horizontal_shear_transform((ALLEGRO_TRANSFORM*)this, DegreesToRadians(degrees));
		al_translate_transform((ALLEGRO_TRANSFORM*)this, origin_x, origin_y);

	}
	void Transform::HorizontalShear(const Point& origin, float degrees) {

		HorizontalShear(origin.X(), origin.Y(), degrees);

	}
	void Transform::VerticalShear(float degrees) {

		al_vertical_shear_transform((ALLEGRO_TRANSFORM*)this, DegreesToRadians(degrees));

	}
	void Transform::VerticalShear(float origin_x, float origin_y, float degrees) {

		al_translate_transform((ALLEGRO_TRANSFORM*)this, -origin_x, -origin_y);
		al_vertical_shear_transform((ALLEGRO_TRANSFORM*)this, DegreesToRadians(degrees));
		al_translate_transform((ALLEGRO_TRANSFORM*)this, origin_x, origin_y);

	}
	void Transform::VerticalShear(const Point& origin, float degrees) {

		VerticalShear(origin.X(), origin.Y(), degrees);

	}
	void Transform::TransformPoint(float& x, float& y) const {

		al_transform_coordinates((ALLEGRO_TRANSFORM*)this, &x, &y);

	}
	void Transform::TransformPoint(Point& point) const {

		float x = point.X();
		float y = point.Y();
		TransformPoint(x, y);
		point.SetXY(x, y);

	}
	Transform& Transform::operator=(const Transform& other) {

		al_identity_transform((ALLEGRO_TRANSFORM*)this);
		al_copy_transform((ALLEGRO_TRANSFORM*)this, (ALLEGRO_TRANSFORM*)&other);

		return *this;

	}

	Point Transform::GetScale() const {

		float scale_x = Sign(_matrix[0][0]) * std::sqrt(std::pow(_matrix[0][0], 2.0f) + std::pow(_matrix[1][0], 2.0f));
		float scale_y = Sign(_matrix[1][1]) * std::sqrt(std::pow(_matrix[0][1], 2.0f) + std::pow(_matrix[1][1], 2.0f));

		return Point(scale_x, scale_y);

	}
	float Transform::GetAngle() const {

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j)
				std::cout << _matrix[j][i] << " ";
			std::cout << std::endl;
		}

		return RadiansToDegrees(std::atan2(_matrix[0][1], _matrix[1][1]));

		//return 0;

	}
	Point Transform::GetOffset() const {

		return Point(_matrix[3][0], _matrix[3][1]);

	}

}

