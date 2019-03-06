#pragma once

#include "hvn3/ecs/ComponentHandle.h"
#include "hvn3/ecs/ComponentPool.h"
#include "hvn3/ecs/ComponentView.h"
#include "hvn3/ecs/IComponent.h"	
#include "hvn3/utility/TypeIndexer.h"

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

namespace hvn3 {
	namespace ecs {

		// A component manager contains one or more component pools, created at runtime.

		class ComponentManager {

			typedef struct default_component_family component_family_type;
			typedef typename TypeIndexer<component_family_type>::index_type index_type;
			typedef EntityId entity_type;

			template<typename ComponentType>
			using pool_type = ComponentPool<ComponentType>;

		public:
			template <typename ComponentType>
			void AddComponent(entity_type entity, ComponentType&& component) {

				// Generate and index for the given type.
				index_type index = _getTypeIndex<ComponentType>();

				if (index >= _pools.size()) {

					// If a pool for this type does not already exist, create it.

					_pools.resize(index + 1);
					_pools[index] = std::make_unique<pool_type<ComponentType>>();

				}

				// Add the component to its respective pool.
				// Note that the component is moved into the pool, and therefore the component instance passed in should not be accessed afterward.
				_getPool<ComponentType>()->Add(entity, std::move(component));

			}

			template<typename ComponentType> typename std::enable_if<!std::is_base_of<IComponent, ComponentType>::value, ComponentHandle<ComponentType>>::type
				GetComponent(entity_type entity) {

				// The component type must inherit from IComponent if it is to be retrieved from one of the dynamic pools (as dynamic cast is used).
				// If it doesn't, we'll simply check its respective pool.

				// Get the pool corresponding to this type.
				pool_type<ComponentType>* pool_ptr = _getPool<ComponentType>();

				// If a pool exists for this type, return the component belonging to the requested entity.
				if (pool_ptr != nullptr)
					return pool_ptr->Find(entity);

				// If the component cannot be found, return a null handle.
				// Some implementations will create the component on request if it hasn't been added yet-- We don't do that here.
				return nullptr;

			}
			template<typename ComponentType> typename std::enable_if<std::is_base_of<IComponent, ComponentType>::value, ComponentHandle<ComponentType>>::type
				GetComponent(entity_type entity) {

				// The component type must inherit from IComponent if it is to be retrieved from one of the dynamic pools (as dynamic cast is used).
				// This overload allows us to look through the dynamic pools to find an appropriate pool to pull the component from (if necessary).

				// Get the pool corresponding to this type.
				pool_type<ComponentType>* pool_ptr = _getPool<ComponentType>();

				// If a pool exists for this type, return the component belonging to the requested entity.
				if (pool_ptr != nullptr)
					return pool_ptr->Find(entity);

				// Otherwise, we must look over the dynamic pools to find a component castable to this type.
				// This is done by checking if the pool contains a component for this entity that inherits from IComponent, and if so, attempts to cast it to the desired type.
				// If this succeeeds, the casted component pointer is returned.

				for (auto i = _pools.begin(); i != _pools.end(); ++i) {

					// If the component pool is null, skip it.
					if (!*i)
						continue;

					// Attempt to find the component belonging to the requested entity.
					IComponent* base_ptr = i->get()->FindAsIComponent(entity);

					if (base_ptr != nullptr) {

						// If we can case it to the requested type, we'll return it.

						ComponentType* derived_ptr = dynamic_cast<ComponentType*>(base_ptr);

						if (derived_ptr != nullptr)
							return derived_ptr;

					}

				}

				// If the component cannot be found, return a null handle.
				// Some implementations will create the component on request if it hasn't been added yet-- We don't do that here.
				return nullptr;

			}

			template<typename ComponentType>
			bool RemoveComponent(entity_type entity) {

				auto pool_ptr = _getPool<ComponentType>();

				if (pool_ptr == nullptr)
					return false;

				return pool_ptr->Remove(entity);

			}
			void RemoveComponents(entity_type entity) {

				for (auto i = _pools.begin(); i != _pools.end(); ++i)
					(*i)->Remove(entity);

			}

			template<typename ComponentType = void>
			size_t Count() const {

				size_t count = 0;

				for (auto i = _pools.begin(); i != _pools.end(); ++i)
					if (_poolContainsType<ComponentType>(i->get()))
						count += (*i)->Count();

				return count;

			}
			template<>
			size_t Count() const {

				size_t count = 0;

				for (auto i = _pools.begin(); i != _pools.end(); ++i)
					count += (*i)->Count();

				return count;

			}

			template<typename ...Components>
			ComponentView<Components...> View() {

				IComponentPool* pool = nullptr;
				bool contains_null_pool = false;

				ComponentView<Components...> view(_getPool<Components>(contains_null_pool)...);

				// If any of the pools were null, the user may be attempting to create a view using a base class.
				// For any pools that were null, add eligible pools to the view's collection of "dynamic" pools.

				if (contains_null_pool)
					int dummy[] = { 0, (_addDynamicPoolToViewIfNull<Components, Components...>(view), 0)... };

				return view;

			}

		private:
			std::vector<std::unique_ptr<IComponentPool>> _pools;

			template<typename ComponentType>
			index_type _getTypeIndex() const {

				return TypeIndexer<component_family_type>::GetIndex<ComponentType>();

			}
			template<typename ComponentType>
			pool_type<ComponentType>* _getPool() const {

				index_type index = _getTypeIndex<ComponentType>();

				if (index < _pools.size() && _pools[index])
					return static_cast<pool_type<ComponentType>*>(_pools[index].get());

				return nullptr;

			}
			template<typename ComponentType>
			pool_type<ComponentType>* _getPool(bool& null) const {

				pool_type<ComponentType>* ptr = _getPool<ComponentType>();

				if (ptr == nullptr)
					null = true;

				return ptr;

			}
			template<typename ComponentType> typename std::enable_if<!std::is_fundamental<ComponentType>::value, bool>::type
				_poolContainsType(IComponentPool* pool) const {

				if (_getPool<ComponentType>() == pool)
					return true;

				if (pool->Count() <= 0)
					return false;

				const IComponent* ptr = pool->AtAsIComponent(0).component;

				if (ptr == nullptr)
					return false;

				const ComponentType* derived_ptr = dynamic_cast<const ComponentType*>(ptr);

				return derived_ptr != nullptr;

			}
			template<typename ComponentType> typename std::enable_if<std::is_fundamental<ComponentType>::value, bool>::type
				_poolContainsType(IComponentPool* pool) const {

				if (_getPool<ComponentType>() == pool)
					return true;

				return false;

			}

			template<typename ComponentType, typename ...ViewType>
			void _addDynamicPoolToViewIfNull(ComponentView<ViewType...>& view) {

				// If the pool is directly available, it would have already been added to the view.

				if (_getPool<ComponentType>() != nullptr)
					return;

				for (auto i = _pools.begin(); i != _pools.end(); ++i)
					if (_poolContainsType<ComponentType>(i->get()))
						view._dynamic_pools.push_back(i->get());

			}

		};

	}
}