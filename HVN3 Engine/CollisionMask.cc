#include "CollisionMask.h"

namespace hvn3 {

	RectangleMask::RectangleMask(const Rectangle<float>& rect) : __mask(rect.Width(), rect.Height()), __offset(rect.X(), rect.Y()) {}
	void RectangleMask::SetOffset(float x_offset, float y_offset) {

		__offset.SetX(__offset.X() + x_offset);
		__offset.SetY(__offset.Y() + y_offset);

	}
	const Point2d<float>& RectangleMask::GetOffset() const {

		return __offset;

	}
	Rectangle<float> RectangleMask::AABB() const {

		Rectangle<float> aabb = __mask;
		aabb.SetX(aabb.X() + __offset.X());
		aabb.SetY(aabb.Y() + __offset.Y());
		return aabb;

	}
	MaskType RectangleMask::Type() const {

		return MaskType::Rectangle;

	}
	bool RectangleMask::Intersects(const Rectangle<float>& rectangle) const {

		return false;

	}
	bool RectangleMask::Intersects(const Circle<float>& circle) const {

		return false;

	}
	bool RectangleMask::Intersects(const Line<float>& line) const {

		return false;

	}

	const PointF CollisionMask::__null_offset = PointF(0.0f, 0.0f);
	CollisionMask::CollisionMask() {}
	CollisionMask::CollisionMask(const Rectangle<float>& rect) {

		__mask = std::make_shared<RectangleMask>(rect);

	}
	void CollisionMask::SetOffset(float x_offset, float y_offset) {
		if (!__mask) return;

		__mask->SetOffset(x_offset, y_offset);

	}
	const Point2d<float>& CollisionMask::GetOffset() const {
		if (!__mask) return __null_offset;

		return __mask->GetOffset();

	}
	Rectangle<float> CollisionMask::AABB() const {
		if (!__mask) return Rectangle<float>(0.0f, 0.0f);

		return __mask->AABB();

	}
	MaskType CollisionMask::Type() const {
		if (!__mask) return MaskType::Undefined;

		return __mask->Type();

	}
	bool CollisionMask::Intersects(const Rectangle<float>& rectangle) const {
		if (!__mask) return false;

		return __mask->Intersects(rectangle);

	}
	bool CollisionMask::Intersects(const Circle<float>& circle) const {
		if (!__mask) return false;

		return __mask->Intersects(circle);

	}
	bool CollisionMask::Intersects(const Line<float>& line) const {
		if (!__mask) return false;

		return __mask->Intersects(line);

	}

	//CollisionMask::CollisionMask() {
	//
	//	__type = UNDEFINED;
	//	__x_offset = 0.0f;
	//	__y_offset = 0.0f;
	//
	//}
	//CollisionMask::CollisionMask(Circle mask) {
	//
	//	__type = CIRCLE;
	//	__radius = mask.Radius();
	//	__x_offset = mask.X;
	//	__y_offset = mask.Y;
	//	__points.push_back(Point(0.0f, 0.0f));
	//
	//}
	//CollisionMask::CollisionMask(Rectangle mask) {
	//
	//	__type = RECTANGLE;
	//	__x_offset = mask.X;
	//	__y_offset = mask.Y;
	//	__points.push_back(Point(0.0f, 0.0f));
	//	__points.push_back(Point(mask.Width(), mask.Height()));
	//
	//}
	//CollisionMask::CollisionMask(int sprite_index) {
	//
	//	__type = SPRITE;
	//	__sprite = Resources::Sprites.Find(sprite_index).get();
	//	__x_offset = __sprite->Origin().X;
	//	__y_offset = __sprite->Origin().Y;
	//	__sprite_mask = SpriteMask(__sprite);
	//
	//}
	//
	//Rectangle CollisionMask::AABB() const {
	//
	//	switch (__type) {
	//
	//	case RECTANGLE:
	//		return Rectangle(__points[0], __points[1]);
	//
	//	case CIRCLE:
	//		return Rectangle(Point(__points[0].X() - __radius, __points[0].Y() - __radius), Point(__points[0].X() + __radius, __points[0].Y() + __radius));
	//
	//	case LINE:
	//		return Rectangle(__points[0].X(), __points[1].Y);
	//
	//	case SPRITE:
	//		return Rectangle(Point(__x_offset, __y_offset), Point(__x_offset + __sprite->Width(), __y_offset + __sprite->Height()));
	//
	//	}
	//
	//	return Rectangle(0.0f, 0.0f);
	//}
	//void CollisionMask::SetOffset(float x_offset, float y_offset) {
	//
	//	__x_offset = x_offset;
	//	__y_offset = y_offset;
	//
	//}
	//Point CollisionMask::GetOffset() const {
	//
	//	return Point(__x_offset, __y_offset);
	//
	//}
	//
	//bool CollisionMask::IsUndefined() const {
	//	return __type == UNDEFINED;
	//}
	//bool CollisionMask::IsRectangle() const {
	//	return __type == RECTANGLE;
	//}
	//bool CollisionMask::IsCircle() const {
	//	return __type == CIRCLE;
	//}
	//bool CollisionMask::IsLine() const {
	//	return __type == LINE;
	//}
	//bool CollisionMask::IsSprite() const {
	//	return __type == SPRITE;
	//}
	//
	//Circle CollisionMask::ToCircle(float x, float y) const {
	//	assert(__type == CIRCLE);
	//
	//	return Circle(x + __x_offset, y + __y_offset, __radius);
	//
	//}
	//Rectangle CollisionMask::ToRectangle(float x, float y) const {
	//	assert(__type == RECTANGLE);
	//
	//	return Rectangle(
	//		Point(x + __x_offset + __points[0].X(), y + __y_offset + __points[0].Y),
	//		Point(x + __x_offset + __points[1].X(), y + __y_offset + __points[1].Y)
	//	);
	//
	//}
	//const SpriteMask& CollisionMask::ToSpriteMask() const {
	//
	//	return __sprite_mask;
	//
	//}

}