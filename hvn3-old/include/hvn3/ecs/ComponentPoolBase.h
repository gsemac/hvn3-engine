#pragma once

#include "hvn3/ecs/IComponentPool.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

namespace hvn3 {
	namespace ecs {

		template<typename ComponentType>
		class ComponentPoolBase :
			public IComponentPool {

		public:
			typedef IComponentPool::entity_type entity_type;
			typedef ComponentType component_type;
			typedef std::vector<EntityComponentPair<ComponentType>> component_vector_type;

			ComponentPoolBase() {

				_sort_required = false;
				_deduplicate_required = false;
				_timestamp = 0;

			}

			ComponentType* Add(entity_type entity, ComponentType&& component) {

				_components.push_back(EntityComponentPair<ComponentType>(entity, std::move(component), _nextTimestamp()));

				// Elements will not necessarily be ordered after adding a new entity.
				_sort_required = true;

				// In case the entity already exists, we will need to deduplicate the collection.
				_deduplicate_required = true;

				return &_components.back().component;

			}
			bool Remove(entity_type entity) override {

				// Erase all components that belong to the given entity.
				// While there should normally only be 1, it's possible that more were added since the last deduplication.
				// Components will still be sorted after removal, so there's no need to resort afterwards.

				// This could be improved by using a faster algorithm in the case that the collection is sorted so we don't need to look at the whole thing.

				auto remove_it = std::remove_if(_components.begin(), _components.end(), [=](const EntityComponentPair<ComponentType>& value) {
					return value.entity == entity;
				});

				if (remove_it == _components.end())
					return false;

				_components.erase(remove_it, _components.end());

				return true;

			}

			bool Exists(entity_type entity) const override {

				return Find(entity) != nullptr;

			}
			size_t Count() const override {

				return _components.size();

			}

			ComponentType* Find(entity_type entity) const {

				_ensureSorted();

				auto it = std::lower_bound(_components.begin(), _components.end(), entity, [](const EntityComponentPair<ComponentType>& lhs, const entity_type rhs) {
					return lhs.entity < rhs;
				});

				if (it == _components.end())
					return nullptr;

				return &it->component;

			}
			EntityComponentPair<ComponentType>& At(size_t index) const {

				_ensureSorted();

				assert(index >= 0);
				assert(index < _components.size());

				return _components[index];

			}

		protected:
			component_vector_type& GetComponents() {
				return _components;
			}
			const component_vector_type& Components() const {
				return _components;
			}

			IComponent* FindAsIComponent(entity_type entity) override {

				// Unless overridden, this function will always return null (support must be added by the derived class).

				return nullptr;

			}
			EntityComponentPair<IComponent*> AtAsIComponent(size_t index) override {

				// Unless overridden, this function will always return null (support must be added by the derived class).

				return EntityComponentPair<IComponent*>(At(index).entity, nullptr, 0);

			}

		private:
			mutable bool _sort_required;
			mutable bool _deduplicate_required;
			int32_t _timestamp;
			mutable component_vector_type _components;

			void _ensureSorted() const {

				if (_sort_required) {

					// Sorts components by entity and timestamp, where components with newer timestamps are listed first.
					// This is so, during the deduplication phase, all but the newest component assigned to each entity is removed.

					std::sort(_components.begin(), _components.end(), [](const EntityComponentPair<ComponentType>& lhs, const EntityComponentPair<ComponentType>& rhs) {
						return (lhs.entity < rhs.entity) && (lhs.timestamp > rhs.timestamp);
					});

				}

				if (_deduplicate_required) {

					// Removes components that exist more than once for the same entity.
					// Make sure that the components with newer timestamps are the ones that are kept.

					_components.erase(std::unique(_components.begin(), _components.end(), [](const EntityComponentPair<ComponentType>& lhs, const EntityComponentPair<ComponentType>& rhs) {
						return lhs.entity == rhs.entity;
					}), _components.end());

				}

				_sort_required = false;
				_deduplicate_required = false;

			}
			int32_t _nextTimestamp() {

				return _timestamp++;

			}

		};

	}
}