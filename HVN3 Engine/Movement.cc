#include <algorithm>
#include <cmath>
#include "Movement.h"

namespace hvn3 {

	bool RotateToAngle(ISpriteable& obj, float angle, float speed) {

		float diff = angle - obj.ImageAngle();

		if ((std::min)(360.0f - std::abs(diff), std::abs(diff)) < speed) {
			obj.SetImageAngle(angle);
			return true;
		}
		else if (diff <= -180.0f || (0.0f < diff && diff < 180.0f))
			obj.SetImageAngle(obj.ImageAngle() + speed);
		else
			obj.SetImageAngle(obj.ImageAngle() - speed);

		obj.SetImageAngle(fmod(obj.ImageAngle() + 360.0f, 360.0f));

		return false;

	}

}