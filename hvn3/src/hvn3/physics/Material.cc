#include "hvn3/physics/Material.h"

namespace hvn3 {
	namespace Physics {

		Material::Material() :
			Material(1.0f, 0.0f) {
		}
		Material::Material(float density, float restitution) :
			density(density),
			restitution(restitution) {
		}

		const Material Material::Rock(0.6f, 0.1f);
		const Material Material::Wood(0.3f, 0.2f);
		const Material Material::Metal(1.2f, 0.05f);
		const Material Material::BouncyBall(0.3f, 0.8f);
		const Material Material::SuperBall(0.3f, 0.95f);
		const Material Material::Pillow(0.1f, 0.2f);
		const Material Material::Static(0.0f, 0.4f);

	}

}