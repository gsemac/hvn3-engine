#include "hvn3/math/GeometryUtils.h"
#include <iomanip>
namespace hvn3 {
	namespace Math {
		namespace Geometry {

			float PointDistanceSquared(const PointF& point, const RectangleF& rectangle) {

				float dx = Math::Max((std::abs)(point.X() - rectangle.X() + rectangle.Width() / 2.0f) - rectangle.Width() / 2.0f, 0.0f);
				float dy = Math::Max((std::abs)(point.Y() - rectangle.Y() + rectangle.Height() / 2.0f) - rectangle.Height() / 2.0f, 0.0f);

				return dx * dx + dy * dy;

			}

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
			float PointDistance(const PointF& point, const RectangleF& rectangle) {

				return (std::sqrtf)(PointDistanceSquared(point, rectangle));

			}

			PointF PointInDirection(const PointF& point, float degrees, float distance) {

				float rad = Math::DegreesToRadians(degrees);
				return PointF(point.X() + std::cos(rad) * distance, point.Y() - std::sin(rad) * distance);

			}
			PointF PointInDirection(const PointF& p1, const PointF& p2, float distance) {

				float deg = PointDirection(p1, p2);
				return PointInDirection(p1, deg, distance);

			}
			PointF PointInDirection(const PointF& point, const Vector2d& direction_vector) {

				return point + direction_vector;

			}

			float PointDirection(float x1, float y1, float x2, float y2) {

				float angle = Math::RadiansToDegrees((std::atan2)(y1 - y2, x2 - x1));

				if (angle < 0.0f)
					angle += 360.0f;

				return angle;

			}
			float PointDirection(const PointF& a, const PointF& b) {

				return PointDirection(a.X(), a.Y(), b.X(), b.Y());

			}

			std::pair<PointF, PointF> TangentThroughPoint(const CircleF& circle, const PointF& point) {

				// Get the vector going from the circle to the desination point.
				Vector2d vec(circle.Position(), point);

				// Calculate the point in the direction of the vector on the edge of the circle.
				PointF point_1 = Math::Geometry::PointInDirection(circle.Position(), vec.Direction(), circle.Radius());
				PointF point_2(point_1);

				// Rotate the points to the tangent point.
				float degrees = Math::RadiansToDegrees((std::acos)(circle.Radius() / vec.Length()));
				point_1 = Math::Geometry::PointRotate(point_1, circle.Position(), -degrees);
				point_2 = Math::Geometry::PointRotate(point_2, circle.Position(), degrees);

				// Return the result.
				return std::pair<PointF, PointF>(point_1, point_2);

			}
			int NumberOfCommonTangents(const CircleF& a, const CircleF& b) {

				// More information:
				// https://www.youtube.com/watch?v=mPBO61d9v3Q

				// Calculate the distance^2 between the centers of the two circles.
				float d = PointDistance(a.Position(), b.Position());
				float rmin = Math::Min(a.Radius(), b.Radius());
				float rmax = Math::Max(a.Radius(), b.Radius());

				// If the two circles are the same circle, they have infinitely many common tangents. Return 0, because this function is not specified for this situation.
				if (a == b)
					return 0;

				// If one circle is completely inside of the other, then there are no common tangents.
				if (rmax > d + rmin)
					return 0;

				// If one circle is inside, but touching the edge of the outer circle, there is one tangent passing through the point of contact.
				if (rmax == d + rmin)
					return 1;

				// If one circle is partially inside of one circle, there are two external tangents.
				if (rmax > d - rmin && rmax < d + rmin)
					return 2;

				// If one circle is outside, but touching the outer edge of the other circle, there is one internal tangent and two external tangents.
				if (d - rmax == rmin)
					return 3;

				// If the two circles are completely separated, there are two internal tangents, and two external tangents.
				return 4;

			}
			CommonTangentsResult CommonTangents(const CircleF& a, const CircleF& b) {

				// More information:
				// http://jwilson.coe.uga.edu/EMAT6680Fa05/Parveen/ASSIGNMENT%206/Assignment%206.htm

				CommonTangentsResult tangents;
				tangents.Count = NumberOfCommonTangents(a, b);

				if (tangents.Count == 0)
					return tangents;

				Vector2d vec(a.Position(), b.Position());
				float direction = vec.Direction();
				auto eq = LineF(a.Position(), b.Position()).GetSlopeInterceptForm();

				if (tangents.Count == 1 || tangents.Count == 3) {

					// Represent the tangent as a line of length 1 between the two circles, with its midpoint at the intersection.
					// If there is 1 tangent, we know for certain that one of the cirles is smaller than the other, and against the inner edge of the outer circle.
					// If there are 3 tangents, we know that one circle is completely outside of the other, touching the outer edge.

					// Get the smaller/larger of the two circles.
					const CircleF *cmin, *cmax;
					if (a < b) {
						cmin = &a;
						cmax = &b;
					}
					else {
						cmin = &b;
						cmax = &a;
					}

					// Get a vector pointing from the center of the larger circle to the center of the larger circle.
					// This is so, in the case that the smaller circle is inside of the larger circle, the vector points towards the point of contact.
					Vector2d vec(cmax->Position(), cmin->Position());
					float direction = vec.Direction();

					// Get the point of contact.
					PointF pcontact = PointInDirection(cmax->Position(), direction, cmax->Radius());

					// Get the endpoints of the line whose midpoint is the point of contact.
					PointF p1 = PointInDirection(pcontact, direction + 90.0f, 0.5f);
					PointF p2 = PointInDirection(pcontact, direction - 90.0f, 0.5f);
					tangents.Tangents[0] = LineF(p1, p2);

				}

				if (tangents.Count >= 2) {

					// Represent the two tangents as lines starting from the point of tangency on the first circle to the point of tangency on the second circle.
					// These will be the external common tangents.

					// We need to find the center of similitude, and then we can find the tangents for each circle passing through this point.
					// We can find this point by taking two parallel lines from the centers of each circle, and getting the intersection point of the line that passes through them
					// with the line passing through the centers of both circles.

					// Get a point on the circle's perimeter that is perpendicular to to the direction of the line connecting the two circles.
					// If the two circles have the same radius, an external center of similitude won't exist. The line that connects these two points will be the tangent instead.
					PointF p1 = PointInDirection(a.Position(), direction + 90.0f, a.Radius());
					PointF p2 = PointInDirection(b.Position(), direction + 90.0f, b.Radius());
					PointF p3 = PointInDirection(a.Position(), direction - 90.0f, a.Radius());
					PointF p4 = PointInDirection(b.Position(), direction - 90.0f, b.Radius());
					auto eq2 = LineF(p1, p2).GetSlopeInterceptForm();

					if (a.Radius() == b.Radius()) {
						tangents.Tangents[tangents.Count == 3 ? 1 : 0] = LineF(p3, p4);
						tangents.Tangents[tangents.Count == 3 ? 2 : 1] = LineF(p1, p2);
					}
					else {
						PointF center_of_similitude = GetIntersectionPoint(eq, eq2);
						PointF tp1 = TangentThroughPoint(a, center_of_similitude).first;
						PointF tp2 = TangentThroughPoint(b, center_of_similitude).first;
						PointF tp3 = TangentThroughPoint(a, center_of_similitude).second;
						PointF tp4 = TangentThroughPoint(b, center_of_similitude).second;
						tangents.Tangents[tangents.Count == 3 ? 1 : 0] = LineF(tp1, tp2);
						tangents.Tangents[tangents.Count == 3 ? 2 : 1] = LineF(tp3, tp4);
					}

				}

				if (tangents.Count == 4) {

					// Represent the two tangents as lines starting from the point of tangency on the first circle to the point of tangency on the second circle.
					// These will be the internal common tangents.

					PointF p1 = PointInDirection(a.Position(), direction + 90.0f, a.Radius());
					PointF p2 = PointInDirection(b.Position(), direction - 90.0f, b.Radius());
					PointF p3 = PointInDirection(a.Position(), direction - 90.0f, b.Radius());
					PointF p4 = PointInDirection(a.Position(), direction + 90.0f, b.Radius());
					auto eq2 = LineF(p1, p2).GetSlopeInterceptForm();

					PointF center_of_similitude = GetIntersectionPoint(eq, eq2);
					PointF tp1 = TangentThroughPoint(a, center_of_similitude).first;
					PointF tp2 = TangentThroughPoint(b, center_of_similitude).first;
					PointF tp3 = TangentThroughPoint(a, center_of_similitude).second;
					PointF tp4 = TangentThroughPoint(b, center_of_similitude).second;
					tangents.Tangents[2] = LineF(tp1, tp2);
					tangents.Tangents[3] = LineF(tp3, tp4);

				}

				return tangents;

			}

		}
	}
}