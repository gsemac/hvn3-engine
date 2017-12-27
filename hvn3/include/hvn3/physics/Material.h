#pragma once

namespace hvn3 {
	namespace Physics {

		struct Material {
			
			Material();
			Material(float density, float restitution);
			
			float density;
			float restitution;

			static const Material Rock;
			static const Material Wood;
			static const Material Metal;
			static const Material BouncyBall;
			static const Material SuperBall;
			static const Material Pillow;
			static const Material Static;

		};

	}
}