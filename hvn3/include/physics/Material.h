#pragma once

namespace hvn3 {
	namespace Physics {

		struct Material {
			
			Material(float density, float restitution);
			
			float Density;
			float Restitution;

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