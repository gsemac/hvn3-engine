//#pragma once
//#include "hvn3/core/IDrawable.h"
//#include "hvn3/core/IManager.h"
//#include "hvn3/core/IUpdatable.h"
//#include "hvn3/core/ManagerDefs.h"
//#include "hvn3/objects/ObjectDefs.h"
//
//#include <functional>
//#include <memory>
//#include <utility>
//
//namespace hvn3 {
//
//	class IObjectManager :
//		public IManager,
//		public IUpdatable,
//		public IDrawable {
//
//	public:
//		virtual ~IObjectManager() = default;
//
//		// Adds a new instance of an object to the manager. The manager assumes ownership.
//		virtual void Add(const IObjectPtr& object) = 0;
//		// Adds a new instance of an object to the manager. The manager assumes ownership.
//		virtual void Add(IObject* object) = 0;
//		// Clears all instances without calling their destroy events.
//		virtual void Clear() = 0;
//		// Calls the destroy event for every instance, and clears all instances.
//		virtual void DestroyAll() = 0;
//
//		// Finds and returns the first instance with the given id, or null if no such instance exists.
//		virtual IObject* Find(ObjectId id) = 0;
//		virtual IObject* FindNext(ObjectId id) = 0;
//
//		virtual size_t Count() const = 0;
//		virtual size_t Count(ObjectId id) const = 0;
//
//		virtual bool Exists(ObjectId id) const = 0;
//
//		virtual void ForEach(const std::function<void(IObjectPtr&)>& func) = 0;
//		virtual void ForEach(const std::function<void(const IObjectPtr&)>& func) const = 0;
//
//		// Constructs an object and adds it to the manager, and returns a pointer to the newly-constructed object.
//		template<typename object_type, typename ... Args>
//		object_type* Create(Args &&... args) {
//
//			object_type* ptr = new object_type(std::forward<Args>(args)...);
//
//			Add(ptr);
//
//			return ptr;
//
//		}
//
//	};
//
//	template<>
//	struct ManagerIdTraits<OBJECT_MANAGER> {
//		typedef IObjectManager type;
//	};
//
//}