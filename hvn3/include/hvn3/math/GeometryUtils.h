#pragma once
#include "hvn3/math/Rectangle.h"
#include "hvn3/math/Circle.h"
#include "hvn3/math/Line.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/math/MathUtils.h"
#include <hvn3/utility/BitFlags.h>

#include <functional>
#include <utility>

namespace hvn3 {
	namespace Math {
		namespace Geometry {

			enum class CohenSutherlandOutCode {
				Inside = 0b0000,
				Left = 0b0001,
				Right = 0b0010,
				Bottom = 0b0100,
				Top = 0b1000
			};
			ENABLE_BITFLAG_OPERATORS(CohenSutherlandOutCode)

				struct CommonTangentsResult {
				CommonTangentsResult() : Tangents{
					LineF(0.0f, 0.0f, 0.0f, 0.0f),
					LineF(0.0f, 0.0f, 0.0f, 0.0f),
					LineF(0.0f, 0.0f, 0.0f, 0.0f),
					LineF(0.0f, 0.0f, 0.0f, 0.0f),
				},
				Count(0) {}
				LineF Tangents[4];
				size_t Count;
			};

			template <typename T>
			struct SlopeIntercept {
				T Slope;
				T Intercept;
			};

			// Returns the distance squared between two points.
			template <typename T>
			T PointDistanceSquared(const Point2d<T>& a, const Point2d<T>& b) {

				T dx = b.X() - a.X();
				T dy = b.Y() - a.Y();

				return dx * dx + dy * dy;

			}
			// Returns the distance squared between a point and a rectangle.
			float PointDistanceSquared(const PointF& point, const RectangleF& rectangle);

			// Returns the distance between two points.
			template <typename T>
			T PointDistance(const Point2d<T>& a, const Point2d<T>& b) {

				return (std::sqrt)(PointDistanceSquared(a, b));

			}
			// Returns the distance from the given point to the given line.
			float PointDistance(const PointF& point, const LineF& line);
			// Returns the distance from the given point to the given rectangle.
			float PointDistance(const PointF& point, const RectangleF& rectangle);

			// Returns the point at the given distance from the starting point in the given direction.
			PointF PointInDirection(const PointF& point, float degrees, float distance);
			// Returns the point at the given distance from the first point in the direction of the second point.
			PointF PointInDirection(const PointF& p1, const PointF& p2, float distance);
			PointF PointInDirection(const PointF& point, const Vector2d& direction_vector);

			// Returns the angle between two points.
			float PointDirection(float x1, float y1, float x2, float y2);
			// Returns the angle between two points.
			float PointDirection(const PointF& a, const PointF& b);

			// Rotates the given point about the origin point by the given number of degrees.
			template<typename T>
			Point2d<T> PointRotate(const Point2d<T>& point, const Point2d<T>& origin, float degrees) {

				Point2d<T> new_point = point;
				float rad = Math::DegreesToRadians(degrees);
				float s = std::sin(rad);
				float c = std::cos(rad);

				// Translate the point to the origin.
				new_point -= origin;

				// Rotate the point.
				float xnew = new_point.X() * c - new_point.Y() * s;
				float ynew = new_point.X() * s + new_point.Y() * c;

				// Translate the point back.
				new_point.SetX(xnew + origin.X());
				new_point.SetY(ynew + origin.Y());

				return new_point;

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

			// Returns the two points of contact of the lines tangent to the circle passing through the given point.
			std::pair<PointF, PointF> TangentThroughPoint(const CircleF& circle, const PointF& point);
			// Returns the number of common tangents between two circles. Assumes that the two circles are not the same circle.
			int NumberOfCommonTangents(const CircleF& a, const CircleF& b);
			CommonTangentsResult CommonTangents(const CircleF& a, const CircleF& b);

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
			// Returns the distance between two circles.
			template <typename T>
			T Distance(const Circle<T>& a, const Circle<T>& b) {

				T dist = PointDistance(a.Position(), b.Position());
				T rsum = a.Radius() + b.Radius();

				if (dist > rsum)
					return dist - rsum;
				else
					return static_cast<T>(0);

			}

			template <typename T>
			Point2d<T> IntersectionPoint(T slope1, T intercept1, T slope2, T intercept2) {

				T x = (intercept2 - intercept1) / (slope1 - slope2);
				T y = (slope1 * x + intercept1);

				return Point2d<T>(x, y);

			}
			template <typename T>
			Point2d<T> IntersectionPoint(const SlopeIntercept<T>& equation1, const SlopeIntercept<T>& equation2) {

				return IntersectionPoint(equation1.Slope, equation1.Intercept, equation2.Slope, equation2.Intercept);

			}
			template <typename T>
			Point2d<T> IntersectionPoint(const Line<T>& line_segment1, const Line<T>& line_segment2) {

				return IntersectionPoint(CalculateSlopeIntercept(line_segment1), CalculateSlopeIntercept(line_segment2));

			}

			template <typename T>
			SlopeIntercept<T> CalculateSlopeIntercept(const Point2d<T>& p1, const Point2d<T>& p2) {

				SlopeIntercept<T> result;

				result.Slope = (p2.Y() - p1.Y()) / (p2.X() - p1.X());
				result.Intercept = -result.Slope * p2.X() + p2.Y();

				return result;

			}
			template <typename T>
			SlopeIntercept<T> CalculateSlopeIntercept(const Line<T>& line_segment) {

				return CalculateSlopeIntercept(line_segment.First(), line_segment.Second());

			}

			template <typename T>
			bool PointIsOnLine(const Line<T>& line, const Point2d<T>& point) {

				T cross_product = (point.Y() - line.First().Y()) * (line.Second().X(), line.First().X()) -
					(point.X() - line.First().X()) * (line.Second().Y() - line.First().Y());

				if (Math::Abs(cross_product) > static_cast<T>(Math::DoubleEpsilon))
					return false;

				T dot_product = (point.X() - line.First().X()) * (line.Second().X() - line.First().X()) + (point.Y() - line.First().Y()) * (line.Second().Y() - line.First().Y());

				if (dot_product < T(0))
					return false;

				T len_sq = (line.Second().X() - line.First().X()) * (line.Second().X() - line.First().X()) + (line.Second().Y() - line.First().Y()) * (line.Second().Y() - line.First().Y());

				if (dot_product > len_sq)
					return false;

				return true;

			}

			template <typename T>
			CohenSutherlandOutCode ComputeCohenSutherlandOutCode(const Rectangle<T>& rectangle, const Point2d<T>& point) {

				CohenSutherlandOutCode out_code = CohenSutherlandOutCode::Inside;

				if (point.X() < rectangle.X())
					out_code |= CohenSutherlandOutCode::Left;
				else if (point.X() > rectangle.X2())
					out_code |= CohenSutherlandOutCode::Right;

				if (point.Y() < rectangle.Y())
					out_code |= CohenSutherlandOutCode::Top;
				else if (point.Y() > rectangle.Y2())
					out_code |= CohenSutherlandOutCode::Bottom;

				return out_code;

			}
			template <typename T>
			std::pair<CohenSutherlandOutCode, CohenSutherlandOutCode> ComputeCohenSutherlandOutCodes(const Rectangle<T>& rectangle, const Line<T>& line) {

				auto result = std::make_pair(ComputeCohenSutherlandOutCode(rectangle, line.First()), ComputeCohenSutherlandOutCode(rectangle, line.Second()));

				return result;

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

				T x1 = a.First().X();
				T x2 = a.Second().X();
				T x3 = b.First().X();
				T x4 = b.Second().X();
				T y1 = a.First().Y();
				T y2 = a.Second().Y();
				T y3 = b.First().Y();
				T y4 = b.Second().Y();

				T det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

				// If the determinant is 0, then there is no intersection.
				if (Math::IsZero(det))
					return false;

				// Calculate the (potential) point of intersection.
				T pre = (x1 * y2 - y1 * x2);
				T post = (x3 * y4 - y3 * x4);
				T x = (pre * (x3 - x4) - (x1 - x2) * post) / det;
				T y = (pre * (y3 - y4) - (y1 - y2) * post) / det;

				// Check that the point is on the lines.
				if (Math::IsLessThan(x, Math::Min(x1, x2)) ||
					Math::IsGreaterThan(x, Math::Max(x1, x2)) ||
					Math::IsLessThan(x, Math::Min(x3, x4)) ||
					Math::IsGreaterThan(x, Math::Max(x3, x4)))
					return false;
				if (Math::IsLessThan(y, Math::Min(y1, y2)) ||
					Math::IsGreaterThan(y, Math::Max(y1, y2)) ||
					Math::IsLessThan(y, Math::Min(y3, y4)) ||
					Math::IsGreaterThan(y, Math::Max(y3, y4)))
					return false;

				return true;

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

				// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

				auto out_codes = ComputeCohenSutherlandOutCodes(a, b);

				// One of the points is inside of the rectangle.
				if (out_codes.first == CohenSutherlandOutCode::Inside || out_codes.second == CohenSutherlandOutCode::Inside)
					return true;

				// Both points on one side of the rectangle (top, left, bottom, or right).
				if (static_cast<int>(out_codes.first & out_codes.second) != 0)
					return false;

				// Line passes through the rectangle (top/bottom or left/right).
				if (((out_codes.first | out_codes.second) == (CohenSutherlandOutCode::Left | CohenSutherlandOutCode::Right)) ||
					((out_codes.first | out_codes.second) == (CohenSutherlandOutCode::Top | CohenSutherlandOutCode::Bottom)))
					return true;

				// At this point, we know one point is to the left or the right, and one is to the top or the bottom.
				// We only need to test the two corresponding edges. 
				Line<T> edge1 = HasFlag(out_codes.first | out_codes.second, CohenSutherlandOutCode::Left) ? a.LeftEdge() : a.RightEdge();
				Line<T> edge2 = HasFlag(out_codes.first | out_codes.second, CohenSutherlandOutCode::Top) ? a.TopEdge() : a.BottomEdge();

				return TestIntersection(b, edge1) || TestIntersection(b, edge2);

			}
			template <typename T>
			bool TestIntersection(const Circle<T>& a, const Circle<T>& b) {

				return PointDistance(Point2d<T>(a.X(), a.Y()), Point2d<T>(b.X(), b.Y())) < (a.Radius() + b.Radius());

			}

		}
	}
}