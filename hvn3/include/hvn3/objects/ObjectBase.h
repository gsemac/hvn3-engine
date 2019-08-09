#pragma once

#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/ecs/EntityManager.h"
#include "hvn3/objects/IObject.h"

#include <limits>

namespace hvn3 {

	template<typename DerivedObjectType>
	class ObjectBase :
		public IObject {

	public:
		ObjectBase();

		IObject::object_id Id() const override;
		ecs::Entity Entity() override;

		bool IsActive() const override;
		void SetActive(bool value) override;

		// Returns the static object ID assigned to this type.
		static IObject::object_id ObjectId();

	protected:
		ApplicationContext Context();

		template<typename ComponentType, typename ...Args>
		void AddComponent(Args&&... args) {

			assert(static_cast<bool>(Context()));
			assert(static_cast<bool>(Entity()));

			auto component_m = Context().Get<ecs::ComponentManager>();

			assert(static_cast<bool>(component_m));

			ComponentType component(std::forward<Args>(args)...);

			component_m->AddComponent(Entity(), std::move(component));

		}

		void OnCreate(CreateEventArgs& e) override;
		void OnDestroy(DestroyEventArgs& e) override;

	private:
		ecs::Entity _entity;
		bool _is_active;

	public:
		ApplicationContext _context;

		void _init();

	};

	// Public methods

	template<typename DerivedObjectType> ObjectBase<DerivedObjectType>::ObjectBase() {

		_init();

	}

	template<typename DerivedObjectType> IObject::object_id ObjectBase<DerivedObjectType>::Id() const {
		return DerivedObjectType::ObjectId();
	}
	template<typename DerivedObjectType> ecs::Entity ObjectBase<DerivedObjectType>::Entity() {

		// If this object has not been assigned an entity ID yet, attempt to get one.

		if (!_entity && Context())
			_entity = Context().Get<ecs::EntityManager>()->CreateEntity();

		return _entity;

	}

	template<typename DerivedObjectType> bool ObjectBase<DerivedObjectType>::IsActive() const {
		return _is_active;
	}
	template<typename DerivedObjectType> void ObjectBase<DerivedObjectType>::SetActive(bool value) {
		_is_active = value;
	}

	template<typename DerivedObjectType> void ObjectBase<DerivedObjectType>::OnCreate(CreateEventArgs& e) {

		// When the object is created, assign it the current context.
		_context = e.Context();

	}
	template<typename DerivedObjectType> void ObjectBase<DerivedObjectType>::OnDestroy(DestroyEventArgs& e) {

		// When the object is destroyed, reset its context.
		_context = ApplicationContext();

	}

	template<typename DerivedObjectType> ApplicationContext ObjectBase<DerivedObjectType>::Context() {
		return _context;
	}

	template<typename DerivedObjectType> IObject::object_id ObjectBase<DerivedObjectType>::ObjectId() {
		return IObject::object_indexer::GetIndex<DerivedObjectType>();
	}

	// Private methods

	template<typename DerivedObjectType> void ObjectBase<DerivedObjectType>::_init() {

		//_is_destroyed = false;
		_is_active = false;

	}

}