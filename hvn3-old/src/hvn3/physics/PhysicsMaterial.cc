#include "hvn3/physics/PhysicsMaterial.h"

namespace hvn3 {

	PhysicsMaterial::PhysicsMaterial() :
		PhysicsMaterial(1.0f, 0.0f) {
	}
	PhysicsMaterial::PhysicsMaterial(float density, float restitution) :
		_density(density),
		_restitution(restitution) {
	}

	float PhysicsMaterial::Density() const {
		return _density;
	}
	float PhysicsMaterial::Restitution() const {
		return _restitution;
	}

	const PhysicsMaterial PhysicsMaterial::Rock(0.6f, 0.1f);
	const PhysicsMaterial PhysicsMaterial::Wood(0.3f, 0.2f);
	const PhysicsMaterial PhysicsMaterial::Metal(1.2f, 0.05f);
	const PhysicsMaterial PhysicsMaterial::BouncyBall(0.3f, 0.8f);
	const PhysicsMaterial PhysicsMaterial::SuperBall(0.3f, 0.95f);
	const PhysicsMaterial PhysicsMaterial::Pillow(0.1f, 0.2f);
	const PhysicsMaterial PhysicsMaterial::Static(0.0f, 0.4f);

}