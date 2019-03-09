#pragma once

#include "hvn3/objects/IObject.h"

#include <limits>

namespace hvn3 {

	template<typename DerivedObjectType>
	class Object :
		public IObject {

	public:
		Object();

		IObject::object_id Id() const override;
		ecs::EntityId EntityId() const override;

		bool IsDestroyed() const override;
		void Destroy() override;
		bool IsActive() const override;
		void SetActive(bool value) override;

		// Returns the static object ID assigned to this type.
		static IObject::object_id ObjectId();

	protected:
		void ReceiveContext(const ApplicationContext& context) override;
		ApplicationContext Context();

		void OnCreate(CreateEventArgs& e) override {};
		void OnDestroy(DestroyEventArgs& e) override {};

	private:
		ecs::EntityId _entity_id;
		bool _is_destroyed;
		bool _is_active;
		ApplicationContext _context;

		void _init();

	};

	// Public methods

	template<typename DerivedObjectType> Object<DerivedObjectType>::Object() {

		_init();

	}
	template<typename DerivedObjectType> IObject::object_id Object<DerivedObjectType>::Id() const {

		return DerivedObjectType::ObjectId();

	}
	template<typename DerivedObjectType> ecs::EntityId Object<DerivedObjectType>::EntityId() const {

		return _entity_id;

	}
	template<typename DerivedObjectType> bool Object<DerivedObjectType>::IsDestroyed() const {

		return _is_destroyed;

	}
	template<typename DerivedObjectType> void Object<DerivedObjectType>::Destroy() {

		_is_destroyed = true;

	}
	template<typename DerivedObjectType> bool Object<DerivedObjectType>::IsActive() const {

		return _is_active;

	}
	template<typename DerivedObjectType> void Object<DerivedObjectType>::SetActive(bool value) {

		_is_active = value;

	}
	template<typename DerivedObjectType> void Object<DerivedObjectType>::ReceiveContext(const ApplicationContext& context) {



	}
	template<typename DerivedObjectType> ApplicationContext Object<DerivedObjectType>::Context() {

		return _context;

	}
	template<typename DerivedObjectType> IObject::object_id Object<DerivedObjectType>::ObjectId() {

		return IObject::object_indexer::GetIndex<DerivedObjectType>();

	}

	// Private methods

	template<typename DerivedObjectType> void Object<DerivedObjectType>::_init() {

		_entity_id = ecs::NULL_ENTITY_ID;
		_is_destroyed = false;
		_is_active = false;

	}

}