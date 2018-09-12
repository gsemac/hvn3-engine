#pragma once
#include <memory>
#include <type_traits>

namespace hvn3 {

	class Collider;
	class ICollider;

	typedef std::shared_ptr<ICollider> IColliderPtr;
	typedef IColliderPtr ColliderPtr;

	template<typename ColliderType = Collider, typename... Args>
	ColliderPtr make_collider(Args&&... args) {

		ColliderPtr ptr = std::make_shared<ColliderType>(std::forward<Args>(args)...);

		return ptr;

	}

}