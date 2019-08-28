#pragma once

#include "hvn3/ecs/Entity.h"
#include "hvn3/ecs/ComponentPool.h"

#include <array>
#include <cassert>
#include <initializer_list>
#include <limits>
#include <tuple>
#include <vector>

namespace hvn3 {
	namespace ecs {

		template<typename ...Components>
		class ComponentView {

			// This is the number of static pools held by the view.
			static constexpr size_t STATIC_POOL_COUNT = sizeof...(Components);

			typedef Entity entity_type;

			template<typename T>
			using pool_type = ComponentPool<T>;

		public:

			// Represents a view of a single entity, allowing access to the components assigned to it (but only those included in this view).
			class EntityView {

			public:
				EntityView() {}
				EntityView(entity_type entity, std::tuple<Components*...>&& components) :
					_current_entity(entity),
					_components(std::move(components)) {

				}

				entity_type Entity() const {

					return _current_entity;

				}
				template<typename Component>
				Component& GetComponent() const {

					return *std::get<Component*>(_components);

				}
				template<std::size_t I>
				typename std::tuple_element<I, std::tuple<Components...>>::type& GetComponent() const {

					return *std::get<I>(_components);

				}

			private:
				entity_type _current_entity;
				std::tuple<Components*...> _components;

			};

			class Iterator {

				// The size of the array used for storing the iterator's current index in each pool.
				static constexpr size_t INDEX_ARRAY_SIZE = sizeof...(Components);

			public:
				Iterator(ComponentView* view, bool end) {

					// The constructor takes the view that the iterator belongs to (in order to access its pools),
					// as well as flag indicating whether or not it is the "end" iterator.

					_view = view;

					// "count" is used to determine if we are at the end of the pools.
					// It is set to the size of the smallest pool. 
					// Since each entity can only appear once in each pool, there can be no entities having all components beyond this point.
					_count = view->_minPoolSize();

					// Initialize the "base pool" to null. This will only be used if the first pool in the view is null.
					// By default, the first pool will be iterated over-- If it is null, we'll find, set, and use the "base pool" instead.
					// Note that it will only be relevant in the case the first pool is a dynamic pool.
					// _base_pool = nullptr;

					if (end)
						_indices[0] = _count + 1;
					else {

						// Fill the index arrays with zero, so we can start with the first element of all pools.
						std::fill(_indices.begin(), _indices.end(), 0);

						// Initialize the current entity view, which is what's returned when the iterator is dereferenced.
						// In other words, make sure we're on the first matching entity.
						_initEntityView();

					}

				}

				EntityView& operator*() {

					return _current_entity;

				}
				const EntityView& operator*() const {

					return _current_entity;

				}
				EntityView* operator->() {

					return &_current_entity;

				}
				const EntityView* operator->() const {

					return &_current_entity;

				}

				bool operator==(const Iterator& other) const {

					return other._indices[0] == _indices[0];

				}
				bool operator!=(const Iterator& other) const {

					return !(*this == other);

				}

				Iterator& operator++() {

					// If we're at the end of the pools, increment one more time to set this iterator equal to the end iterator (i.e., one over the end).

					if (_indices[0] == _count)
						++_indices[0];

					while (_indices[0] < _count) {

						bool has_null_component = false;
						entity_type entity = _getNextEntityFromStaticPools();
						size_t index_index = 0; // the index of the current pool's index in the index array

						// Construct a tuple containing all components belonging to this entity.
						// In the process, we store whether or not we hav come across any null components (which occurs if an entity does not have one of the requested components).
						// When this happens, the entity will simply be skipped.
						std::tuple<Components*...> components{ _view->_getComponentFromIndex<Components>(entity, _indices[index_index++], has_null_component)... };

						if (!has_null_component) {

							// If the entity does not have any null components, we'll move the tuple into an entity view and exit the loop.
							_current_entity = EntityView(entity, std::move(components));

							break;

						}
						else if (_indices[0] == _count)
							// We only increment the base pool index on failure because it would have already been incremented when we were pulling out the components.
							// We increment it one more time so that it can be successfully compared to the end iterator (one over the end of the range).
							++_indices[0];

					}

					return *this;

				}

			private:
				ComponentView* _view;
				EntityView _current_entity;
				std::array<size_t, INDEX_ARRAY_SIZE> _indices;
				size_t _count;

				void _initEntityView() {

					++(*this);

				}
				entity_type _getNextEntityFromStaticPools() const {

					// If the first pool is directly accessible, just access it directly to get the next entity.
					// For now, require that this be the case.

					assert(std::get<0>(_view->_pools) != nullptr);

					return std::get<0>(_view->_pools)->At(_indices[0]).entity;

					// Otherwise, get the next entity from the first non-null pool.

					/*if (_base_pool == nullptr) {

						std::array<IComponentPool*, STATIC_POOL_COUNT> pools = { std::get<pool_type<Components>*>(_view->_pools)... };
						for (auto i = pools.begin(); i != pools.end(); ++i)
							if (*i != nullptr) {

								_base_pool = *i;

								break;

							}
					}

					return _base_pool->AtAsIComponent(_indices[0]).entity;*/

				}

			};

			ComponentView(pool_type<Components>*... args) {

				static_assert(sizeof...(Components) > 0, "View must have at least one pool.");

				_pools = std::make_tuple(args...);

			}

			EntityView GetComponents(EntityId id) {

				std::tuple<Components*...> tuple;

				tuple = std::make_tuple(_getComponent<Components>(id)...);

				return EntityView(id, std::move(tuple));

			}

			Iterator begin() {
				return Iterator(this, false);
			}
			Iterator end() {
				return Iterator(this, true);
			}

			template<typename Func>
			void ForEach(Func callback) {

				for (auto i = begin(); i != end(); ++i)
					callback(i->GetComponent<Components>()...);

			}

		public:
			std::tuple<pool_type<Components>*...> _pools;
			std::vector<IComponentPool*> _dynamic_pools;

			template<typename ComponentType> typename std::enable_if<std::is_base_of<IComponent, ComponentType>::value, ComponentType*>::type
				_getComponent(EntityId id) {

				using pool_type = pool_type<ComponentType>;

				// Get the pool corresponding to this component.

				pool_type* pool = std::get<pool_type*>(_pools);

				// Get the component corresponding to this entity.

				if (pool != nullptr)
					return pool->Find(id);

				return nullptr;


			}
			template<typename ComponentType> typename std::enable_if<!std::is_base_of<IComponent, ComponentType>::value, ComponentType*>::type
				_getComponent(entity_type entity) {

				using pool_type = pool_type<ComponentType>;

				// Get the pool corresponding to this component.

				pool_type* pool = std::get<pool_type*>(_pools);

				// Get the component corresponding to this entity.

				if (pool != nullptr)
					return pool->Find(entity);

				// If the pool was null, check the "dynamic" pools, if available, for the requested type.

				for (auto i = _dynamic_pools.begin(); i != _dynamic_pools.end(); ++i) {

					IComponent* interface_ptr = (*i)->FindAsIComponent(entity);

					if (interface_ptr == nullptr)
						continue;

					ComponentType* derived_ptr = dynamic_cast<ComponentType*>(interface_ptr);

					if (derived_ptr != nullptr)
						return derived_ptr;

				}

				return nullptr;


			}

			template<typename ComponentType> typename std::enable_if<std::is_base_of<IComponent, ComponentType>::value, ComponentType*>::type
				_getComponentFromIndex(entity_type entity, size_t& index, bool& null) {

				assert(index >= 0);

				using pool_type = pool_type<ComponentType>;

				pool_type* pool = std::get<pool_type*>(_pools);

				if (pool != nullptr && index < pool->Count()) {

					auto& entity_component = pool->At(index);

					while (entity_component.entity < entity)
						entity_component = pool->At(++index);

					if (entity_component.entity == entity) {

						++index;

						return &entity_component.component;

					}

				}

				null = true;

				return nullptr;

			}
			template<typename ComponentType> typename std::enable_if<!std::is_base_of<IComponent, ComponentType>::value, ComponentType*>::type
				_getComponentFromIndex(entity_type entity, size_t& index, bool& null) {

				assert(index >= 0);

				using pool_type = pool_type<ComponentType>;

				pool_type* pool = std::get<pool_type*>(_pools);

				if (pool != nullptr) {

					if (index < pool->Count()) {

						auto& entity_component = pool->At(index);

						while (entity_component.entity < entity)
							entity_component = pool->At(++index);

						if (entity_component.entity == entity) {

							++index;

							return &entity_component.component;

						}

					}

				}
				else if (_dynamic_pools.size() > 0) {

					// The user is probably trying to access the component through its base class.
					// Search through the "dynamic" pools for an appropriate one to use.

					for (auto i = _dynamic_pools.begin(); i != _dynamic_pools.end(); ++i) {

						IComponent* icomponent = (*i)->FindAsIComponent(entity);

						if (icomponent == nullptr)
							continue;

						ComponentType* component = dynamic_cast<ComponentType*>(icomponent);

						if (component == nullptr)
							continue;

						++index;

						return component;

					}

				}

				null = true;

				return nullptr;

			}

			size_t _minPoolSize() const {

				size_t min = std::min({ (std::get<pool_type<Components>*>(_pools) == nullptr ? std::numeric_limits<size_t>::max() : std::get<pool_type<Components>*>(_pools)->Count())... });

				if (min == std::numeric_limits<size_t>::max())
					min = 0;

				return min;

			}

		};

	}
}