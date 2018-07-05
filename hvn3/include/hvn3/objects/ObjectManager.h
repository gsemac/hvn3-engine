#pragma once
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/objects/IObjectManager.h"
#include <memory>
#include <vector>

namespace hvn3 {

	class ObjectManager : 
		public IObjectManager,
		public System::IContextReceiver {

		struct ObjectListItem {

			ObjectListItem(ObjectPtr& object);

			ObjectPtr object;
			// This is set to false when the OnCreate event has been called.
			bool callOnCreateEvent;
			// This is set to false when the OnDestroy event has been called.
			bool callOnDestroyEvent;
		
		};
		typedef std::vector<ObjectListItem> object_list_type;

	public:
		ObjectManager();

		IObject* Add(ObjectPtr& object) override;
		IObject* Add(IObject* object) override;
		void Clear() override;
		void DestroyAll() override;

		IObject* Find(ObjectId id) override;
		IObject* FindNext(ObjectId id) override;
		size_t Count() const override;
		size_t Count(ObjectId id) const override;
		bool Exists(ObjectId id) const override;

		void OnBeginUpdate(UpdateEventArgs& e) override;
		void OnUpdate(UpdateEventArgs& e) override;
		void OnEndUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

	protected:
		void OnContextChanged(ContextChangedEventArgs& e) override;

	private:
		object_list_type _objects;
		ObjectId _last_found_id;
		size_t _last_found_index;
		Context _context;

		void _removeDestroyedObjects(object_list_type::iterator begin, object_list_type::iterator end);
		// Updates the state of the given object, and calls the OnCreate or OnDestroy event where applicable. Returns true if the object should have its OnUpdate events called; returns false otherwise.
		bool _updateAndCheckObject(ObjectListItem& item, UpdateEventArgs& e);

	};

}