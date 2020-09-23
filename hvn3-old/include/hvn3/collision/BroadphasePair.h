#pragma once

#include "hvn3/components/ColliderComponent.h"

namespace hvn3 {

	class BroadphasePair {

	public:
		class Element {

		public:
			ColliderComponent* collider;
			TransformComponent* transform;

			Element() :
				collider(nullptr),
				transform(nullptr) {
			}
			Element(TransformComponent* transform, ColliderComponent* collider) :
				collider(collider),
				transform(transform) {
			}

			ColliderComponent& GetCollider() {
				return *collider;
			}
			TransformComponent& GetTransform() {
				return *transform;
			}

		};

		typedef Element value_type;

		value_type first;
		value_type second;

		BroadphasePair(const value_type& first, const value_type& second) :
			first(first),
			second(second) {
		}

		value_type& First() {
			return first;
		}
		value_type& Second() {
			return second;
		}

	};

}