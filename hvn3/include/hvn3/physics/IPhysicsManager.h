//#pragma once
//#include "hvn3/core/IManager.h"
//#include "hvn3/core/IUpdatable.h"
//#include "hvn3/math/Vector2d.h"
//#include "hvn3/physics/PhysicsDefs.h"
//
//#include <cstdint>
//
//namespace hvn3 {
//
//	class ICollider;
//
//	namespace Physics {
//
//		class IPhysicsBody;
//
//		class IPhysicsManager :
//			public IManager,
//			public IUpdatable {
//			
//		public:
//			virtual ~IPhysicsManager() = default;
//
//			virtual void Add(IPhysicsBodyPtr& body) = 0;
//			virtual size_t Count() const = 0;
//			virtual void Clear() = 0;
//
//			virtual const Vector2d& Gravity() const = 0;
//			virtual void SetGravity(const Vector2d& value) = 0;
//			virtual float PixelsToMetersScale() const = 0;
//			virtual void SetPixelsToMetersScale(float value) = 0;
//
//		};
//
//	}
//
//	template<>
//	struct ManagerIdTraits<PHYSICS_MANAGER> {
//		typedef Physics::IPhysicsManager type;
//	};
//
//}