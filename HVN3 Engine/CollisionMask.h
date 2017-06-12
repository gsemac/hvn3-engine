#pragma once
#include <vector>
#include <memory>
#include "IDrawable.h"
#include "SpriteMask.h"

namespace hvn3 {

	enum class MaskType {
		Undefined,
		Rectangle,
		Circle,
		Line,
		Polygon,
		Sprite,
		Composite
	};

	class ICollisionMask {

	public:
		virtual void SetOffset(float x_offset, float y_offset) = 0;
		virtual const Point2d<float>& GetOffset() const = 0;

		virtual Rectangle<float> AABB() const = 0;
		virtual MaskType Type() const = 0;

		virtual bool Intersects(const Rectangle<float>& rectangle) const = 0;
		virtual bool Intersects(const Circle<float>& circle) const = 0;
		virtual bool Intersects(const Line<float>& line) const = 0;

	};

	class RectangleMask : public ICollisionMask {

	public:
		RectangleMask(const Rectangle<float>& rect);

		void SetOffset(float x_offset, float y_offset) override;
		const Point2d<float>& GetOffset() const override;

		Rectangle<float> AABB() const override;
		MaskType Type() const override;

		bool Intersects(const Rectangle<float>& rectangle) const override;
		bool Intersects(const Circle<float>& circle) const override;
		bool Intersects(const Line<float>& line) const override;

	private:
		Point2d<float> __offset;
		Rectangle<float> __mask;

	};

	class CollisionMask : public ICollisionMask {

	public:
		CollisionMask();
		CollisionMask(const Rectangle<float>& rect);

		void SetOffset(float x_offset, float y_offset) override;
		const Point2d<float>& GetOffset() const override;

		Rectangle<float> AABB() const override;
		MaskType Type() const override;

		bool Intersects(const Rectangle<float>& rectangle) const override;
		bool Intersects(const Circle<float>& circle) const override;
		bool Intersects(const Line<float>& line) const override;

	private:
		static const Point2d<float> __null_offset;
		std::shared_ptr<ICollisionMask> __mask;

	};

	//class CollisionMask {
	//
	//private:
	//	enum TYPE {
	//		UNDEFINED,
	//		RECTANGLE,
	//		CIRCLE,
	//		LINE,
	//		POLYGON,
	//		SPRITE,
	//		COMPOSITE
	//	};
	//
	//	TYPE __type;
	//	std::vector<Point> __points;
	//	float __x_offset, __y_offset;
	//	float __radius; // Circle mask only
	//	Sprite* __sprite; // Sprite mask only
	//
	//public:
	//	SpriteMask __sprite_mask; // Sprite mask only
	//	CollisionMask();
	//	CollisionMask(Circle mask);
	//	CollisionMask(Rectangle mask);
	//	CollisionMask(int sprite_index);
	//	Rectangle AABB() const;
	//	void SetOffset(float x_offset, float y_offset);
	//	Point GetOffset() const;
	//
	//	bool IsUndefined() const;
	//	bool IsRectangle() const;
	//	bool IsCircle() const;
	//	bool IsLine() const;
	//	bool IsSprite() const;
	//
	//	Circle ToCircle(float x, float y) const;
	//	Rectangle ToRectangle(float x, float y) const;
	//	Line ToLine(float x, float y) const;
	//	const SpriteMask& ToSpriteMask() const;
	//
	//};

}