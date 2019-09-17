#pragma once
#include "hvn3/math/Rectangle.h"
#include "hvn3/math/Circle.h"
#include "hvn3/math/Line.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/math/MathUtils.h"
#include <hvn3/utility/BitFlags.h>

#include <array>
#include <functional>
#include <utility>

namespace hvn3 {
	namespace Math {
		namespace Geometry {

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

			template<typename ValueType, int SIZE>
			struct IntersectionPointsResult {

				typedef std::array<Point2d<ValueType>, SIZE> array_type;

				bool infinite;
				int count;
				array_type points;

				IntersectionPointsResult() {

					infinite = false;
					count = 0;

				}

				typename array_type::const_iterator begin() const {
					return points.begin();
				}
				typename array_type::const_iterator end() const {
					return points.begin() + (std::min(count, SIZE));
				}

				explicit operator bool() const {
					return count > 0 || infinite;
				}

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

			

			template<typename ValueType>
			IntersectionPointsResult<ValueType, 1> GetIntersectionPoints(const Line<ValueType>& line1, const Line<ValueType>& line2) {

				IntersectionPointsResult<ValueType, 1> result;

				auto s1 = line1.GetStandardForm();
				auto s2 = line2.GetStandardForm();

				ValueType determinant = (s1.A * s2.B) - (s2.A * s1.B);
				Point2d<ValueType> point;

				if (Math::IsZero(determinant)) {

					// The lines are parallel. If one line lines on the other, they intersect.
					// Take whichever end point is closest to the start point of the first line.

					float dist_sq = std::numeric_limits<float>::max();

					if (line1.ContainsPoint(line2.First())) {

						point = line2.First();
						dist_sq = Math::Geometry::PointDistanceSquared(line1.First(), line2.First());

						result.count = 1;

					}

					if (line1.ContainsPoint(line2.Second())) {

						float try_dist_sq = Math::Geometry::PointDistanceSquared(line1.First(), line2.Second());

						if (try_dist_sq < dist_sq) {

							point = line2.Second();
							try_dist_sq = dist_sq;

							result.count = 1;

						}


					}

					result.points[0] = point;
					result.infinite = true;

					return result;

				}

				// Calculate the potential point of intersection.

				point.x = (s2.B * s1.C - s1.B * s2.C) / determinant;
				point.y = (s1.A * s2.C - s2.A * s1.C) / determinant;

				// Make sure that the point is on both of the line segments.

				// Don't kid yourself into thinking these IsGreaterThan/IsLessThan calls are unnecessary.
				// Comparing with epsilon is vital for floating point types; a lot of cases will be erroneously rejected otherwise.

				if (Math::IsGreaterThan((std::min)(line1.X(), line1.X2()), point.x) || Math::IsLessThan((std::max)(line1.X(), line1.X2()), point.x) ||
					Math::IsGreaterThan((std::min)(line1.Y(), line1.Y2()), point.y) || Math::IsLessThan((std::max)(line1.Y(), line1.Y2()), point.y) ||
					Math::IsGreaterThan((std::min)(line2.X(), line2.X2()), point.x) || Math::IsLessThan((std::max)(line2.X(), line2.X2()), point.x) ||
					Math::IsGreaterThan((std::min)(line2.Y(), line2.Y2()), point.y) || Math::IsLessThan((std::max)(line2.Y(), line2.Y2()), point.y))
					return result;

				// The point is on both lines, so we have an intersection.
				result.points[0] = point;
				result.count = 1;

				return result;

			}

			template<typename ValueType>
			Point2d<ValueType> GetIntersectionPoints(const LineSlopeInterceptForm<ValueType>& line1, const LineSlopeInterceptForm<ValueType>& line2) {

				Point2d<ValueType> point;

				point.x = (line2.intercept - line1.intercept) / (line1.slope - line2.slope);
				point.y = (line1.slope * point.x + line1.intercept);

				return point;

			}

			template<typename ValueType>
			IntersectionPointsResult<ValueType, 2> GetIntersectionPoints(const Line<ValueType>& line, const Rectangle<ValueType>& rect) {

				IntersectionPointsResult<ValueType, 2> result;

				// If the line is collinear with one of the rectangle's edges, there are infinite intersection points.
				if (line.X() == line.X2() && (line.X() == rect.Left() || line.X() == rect.Right()) ||
					line.Y() == line.Y2() && (line.Y() == rect.Top() || line.Y() == rect.Bottom())) {

					result.infinite = true;

					return result;

				}

				auto out_codes = GetCohenSutherlandOutCodes(rect, line);

				// Check if both points are inside of the rectangle. If so, there are no intersection points.
				if (static_cast<int>(out_codes.first | out_codes.second) == 0)
					return result;

				// Check if both points are on the same side of the rectangle. If so, there are no intersection points.
				if (static_cast<int>(out_codes.first & out_codes.second) != 0)
					return result;

				// Get the intersection point closest to the first point of the line.

				if (out_codes.first != CohenSutherlandOutCode::Inside) {

					IntersectionPointsResult<ValueType, 1> test;
					CohenSutherlandOutCode out_code = out_codes.first;

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Top))
						test = GetIntersectionPoints(line, rect.TopEdge());

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Left))
						test = GetIntersectionPoints(line, rect.LeftEdge());

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Right))
						test = GetIntersectionPoints(line, rect.RightEdge());

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Bottom))
						test = GetIntersectionPoints(line, rect.BottomEdge());

					if (test) {
						result.points[result.count++] = test.points[0];
						result.infinite = test.infinite;
					}

				}

				// Get the intersection point closest to the second point of the line.

				if (out_codes.second != CohenSutherlandOutCode::Inside) {

					IntersectionPointsResult<ValueType, 1> test;
					CohenSutherlandOutCode out_code = out_codes.second;

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Top))
						test = GetIntersectionPoints(line, rect.TopEdge());

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Left))
						test = GetIntersectionPoints(line, rect.LeftEdge());

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Right))
						test = GetIntersectionPoints(line, rect.RightEdge());

					if (!test && HasFlag(out_code, CohenSutherlandOutCode::Bottom))
						test = GetIntersectionPoints(line, rect.BottomEdge());

					if (test) {
						result.points[result.count++] = test.points[0];
						result.infinite = test.infinite;
					}

				}

				return result;

			}
			template<typename ValueType>
			IntersectionPointsResult<ValueType, 2> GetIntersectionPoints(const Rectangle<ValueType>& rect, const Line<ValueType>& line) {
				return GetIntersectionPoints(line, rect);
			}

		}
	}
}