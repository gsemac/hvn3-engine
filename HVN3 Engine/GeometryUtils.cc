#include "GeometryUtils.h"

namespace hvn3 {
	namespace Math {
		namespace Geometry {

			float PointDistanceSquared(const PointF& a, const PointF& b) {

				float dx = b.X() - a.X();
				float dy = b.Y() - a.Y();

				return dx * dx + dy * dy;

			}
			float PointDistanceSquared(const PointF& point, const RectangleF& rectangle) {

				float dx = Math::Max((std::abs)(point.X() - rectangle.X() + rectangle.Width() / 2.0f) - rectangle.Width() / 2.0f, 0.0f);
				float dy = Math::Max((std::abs)(point.Y() - rectangle.Y() + rectangle.Height() / 2.0f) - rectangle.Height() / 2.0f, 0.0f);

				return dx * dx + dy * dy;

			}

			float PointDistance(const PointF& a, const PointF& b) {

				return std::sqrt(PointDistanceSquared(a, b));

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

			float PointDirection(float x1, float y1, float x2, float y2) {

				float angle = Math::RadiansToDegrees((std::atan2)(y1 - y2, x2 - x1));

				if (angle < 0.0f)
					angle += 360.0f;

				return angle;

			}
			float PointDirection(const PointF& a, const PointF& b) {

				return PointDirection(a.X(), a.Y(), b.X(), b.Y());

			}

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

		}
	}
}