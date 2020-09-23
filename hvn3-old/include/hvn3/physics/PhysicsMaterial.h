#pragma once

namespace hvn3 {
	
	class PhysicsMaterial {

	public:
		PhysicsMaterial();
		PhysicsMaterial(float density, float restitution);

		float Density() const;
		float Restitution() const;

		static const PhysicsMaterial Rock;
		static const PhysicsMaterial Wood;
		static const PhysicsMaterial Metal;
		static const PhysicsMaterial BouncyBall;
		static const PhysicsMaterial SuperBall;
		static const PhysicsMaterial Pillow;
		static const PhysicsMaterial Static;

	private:
		float _density;
		float _restitution;

	};

}