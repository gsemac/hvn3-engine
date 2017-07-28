#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "MathUtils.h"

namespace hvn3 {

	namespace Math {
		namespace Geometry {

			// Returns the distance squared between two points.
			float PointDistanceSquared(const PointF& a, const PointF& b) {

				float dx = b.X() - a.X();
				float dy = b.Y() - a.Y();

				return dx * dx + dy * dy;

			}
			// Returns the distance squared between a point and a rectangle.
			float PointDistanceSquared(const PointF& point, const RectangleF& rectangle) {

				float dx = Math::Max((std::abs)(point.X() - rectangle.X() + rectangle.Width() / 2.0f) - rectangle.Width() / 2.0f, 0.0f);
				float dy = Math::Max((std::abs)(point.Y() - rectangle.Y() + rectangle.Height() / 2.0f) - rectangle.Height() / 2.0f, 0.0f);

				return dx * dx + dy * dy;

			}

			// Returns the distance between two points.
			float PointDistance(const PointF& a, const PointF& b) {

				return std::sqrt(PointDistanceSquared(a, b));

			}
			// Returns the distance from the given point to the given line.
			float PointDistance(const PointF& point, const LineF& line) {

				float a = point.X() - line.First().X();
				float b = point.Y() - line.First().Y();
				float c = line.Second().X() - line.First().X();
				float d = line.Second().Y() - line.First().Y();

				float dot = a * c + b * d;
				float len_sq = c * c + d * d;
				float param = -1.0f;
				if (len_sq != 0)
					param = dot / len_sq;

				float xx, yy;

				if (param < 0) {
					xx = line.First().X();
					yy = line.First().Y();
				}
				else if (param > 1) {
					xx = line.Second().X();
					yy = line.Second().Y();
				}
				else {
					xx = line.First().X() + param * c;
					yy = line.First().Y() + param * d;
				}

				float dx = point.X() - xx;
				float dy = point.Y() - yy;

				return std::sqrt(dx * dx + dy * dy);

			}
			// Returns the distance from the given point to the given rectangle.
			float PointDistance(const PointF& point, const RectangleF& rectangle) {

				return (std::sqrtf)(PointDistanceSquared(point, rectangle));

			}

			// Returns the point at the given distance from the starting point in the given direction.
			PointF PointInDirection(const PointF& point, float degrees, float distance) {

				float rad = Math::DegreesToRadians(degrees);
				return PointF(point.X() + std::cos(rad) * distance, point.Y() - std::sin(rad) * distance);

			}
			
			// Returns the angle between two points.
			float PointDirection(float x1, float y1, float x2, float y2) {

				float angle = Math::RadiansToDegrees((std::atan2)(y1 - y2, x2 - x1));

				if (angle < 0.0f) 
					angle += 360.0f;

				return angle;

			}
			// Returns the angle between two points.
			float PointDirection(const PointF& a, const PointF& b) {

				return PointDirection(a.X(), a.Y(), b.X(), b.Y());

			}

			// Rotates the given point about the origin point by the given number of degrees.
			void PointRotate(PointF& point, const PointF& origin, float degrees) {

				float rad = Math::DegreesToRadians(degrees);
				float s = std::sin(rad);
				float c = std::cos(rad);

				// Translate the Point to the origin.
				point -= origin;

				// Rotate the Point.
				float xnew = point.X() * c - point.Y() * s;
				float ynew = point.X() * s + point.Y() * c;

				// Translate the Point back.
				point.SetX(xnew + origin.X());
				point.SetY(ynew + origin.Y());

			}

			// Returns true if the given point is inside of the given rectangle.
			template <typename T>
			bool PointIn(const Point2d<T>& point, const Rectangle<T>& rect) {

				return (point.X() >= rect.X() && point.X() < rect.X2() && point.Y() >= rect.Y() && point.Y() < rect.Y2());

			}
			// Returns true if the given point is inside of the given circle.
			template <typename T>
			bool PointIn(const Point2d<T>& point, const Circle<T>& circle) {

				return PointDistanceSquared(point, circle.Position()) < (circle.Radius() * circle.Radius());

			}
			
			// Translates the given point by an offset.
			template <typename T>
			Positionable2dBase<T>& TranslatePoint(Positionable2dBase<T>& obj, T x_offset, T y_offset) {

				obj.SetX(obj.X() + x_offset);
				obj.SetY(obj.Y() + y_offset);

				return obj;

			}

			// Returns the distance between two rectangles.
			template <typename T>
			float Distance(const Rectangle<T>& a, const Rectangle<T>& b) {

				// If the two rectangles intersect, the distance between them is zero.
				if (Intersects(a, b)) return 0.0f;

				// Otherwise, the distance is the distance to the closest corner of the other rectangle.
				bool left = b.Right() < a.Left(); // b is on the left of a
				bool right = b.Left() > a.Right(); // b is on the right of a
				bool bottom = b.Top() > a.Bottom(); // b is below a
				bool top = b.Bottom() < a.Top(); // b is above a

				if (top && left)
					return PointDistance(b.BottomRight(), a.TopLeft());
				else if (left && bottom)
					return PointDistance(b.TopRight(), a.BottomLeft());
				else if (bottom && right)
					return PointDistance(b.TopLeft(), a.BottomRight());
				else if (right && top)
					return PointDistance(b.BottomLeft(), a.TopRight());
				else if (left)
					return a.Left() - b.Right();
				else if (right)
					return b.Left() - a.Right();
				else if (bottom)
					return b.Top() - a.Bottom();
				else
					return a.Top() - b.Bottom();

			}

			template <typename T>
			bool TestIntersection(const Rectangle<T>& a, const Rectangle<T>& b) {

				return (a.X() < b.X2() && a.X2() > b.X() && a.Y() < b.Y2() && a.Y2() > b.Y());

			}
			template <typename T>
			bool TestIntersection(const Circle<T>& a, const Line<T>& b) {

				// Note: This procedure uses the same logic as PointDistance, but avoids using the costly sqrt function.

				float _a = a.X() - b.First().X();
				float _b = a.Y() - b.First().Y();
				float _c = b.Second().X() - b.First().X();
				float _d = b.Second().Y() - b.First().Y();

				float dot = _a * _c + _b * _d;
				float len_sq = _c * _c + _d * _d;
				float param = -1.0f;
				if (len_sq != 0)
					param = dot / len_sq;

				float xx, yy;

				if (param < 0) {
					xx = b.First().X();
					yy = b.First().Y();
				}
				else if (param > 1) {
					xx = b.Second().X();
					yy = b.Second().Y();
				}
				else {
					xx = b.First().X() + param * _c;
					yy = b.First().Y() + param * _d;
				}

				float dx = a.X() - xx;
				float dy = a.Y() - yy;

				return (dx * dx + dy * dy) < (std::pow)(a.Radius(), 2.0f);

				// return PointDistance(Point(a.X(), a.Y), b) <= a.Radius();

			}
			template <typename T>
			bool TestIntersection(const Line<T>& a, const Line<T>& b) {

				throw NotImplementedException();

			}
			template <typename T>
			bool TestIntersection(const Rectangle<T>& a, const Circle<T>& b) {

				return PointIn(Point2d<T>(b.X(), b.Y()), a) ||
					TestIntersection(b, Line<T>(a.X(), a.Y(), a.X2(), a.Y())) || // top
					TestIntersection(b, Line<T>(a.X(), a.Y2(), a.X2(), a.Y2())) || // bottom 
					TestIntersection(b, Line<T>(a.X(), a.Y(), a.X(), a.Y2())) || // left
					TestIntersection(b, Line<T>(a.X2(), a.Y(), a.X2(), a.Y2())); // right

			}
			template <typename T>
			bool TestIntersection(const Rectangle<T>& a, const Line<T>& b) {

				throw System::NotImplementedException();

			}
			template <typename T>
			bool TestIntersection(const Circle<T>& a, const Circle<T>& b) {

				return PointDistance(Point2d<T>(a.X(), a.Y()), Point2d<T>(b.X(), b.Y())) < (a.Radius() + b.Radius());

			}



		}
	}
}