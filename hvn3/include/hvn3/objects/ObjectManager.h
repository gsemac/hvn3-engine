#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/objects/IObjectManager.h"

#include <memory>
#include <utility>
#include <vector>

namespace hvn3 {

	class ObjectManager :
		public ManagerBase<IObjectManager>,
		public EventListenerBase<events::UpdateEvents> {

		struct ObjectInfo {

			ObjectInfo(std::shared_ptr<IObject>&& object);
			//ObjectInfo(ObjectInfo&& other);

			//ObjectInfo& operator=(ObjectInfo& other);

			std::shared_ptr<IObject> object;
			bool callCreateEvent;
			bool callDestroyEvent;
			bool destroyed;

		};

	public:
		ObjectManager();

		template<typename ObjectType, typename ...Args>
		void AddObject(Args&&... args) {

			std::shared_ptr<IObject> object = std::make_shared<ObjectType>(std::forward<Args>(args)...);

			_addObject(std::move(object));
		}

		bool RemoveObject(ObjectHandle handle);
		bool RemoveObject(IObject* object);
		template<typename ObjectType>
		bool RemoveObject() {
			return _removeObject(ObjectType::ObjectId());
		}

		void OnStart(StartEventArgs& e) override;
		void OnEnd(EndEventArgs& e) override;

		void Clear() override;
		void DestroyAll() override;

		ObjectHandle Find(IObject::object_id id) override;
		bool Exists(IObject::object_id id) const override;

		size_type Count() const override;
		size_type Count(IObject::object_id id) const override;

		void OnEvent(UpdateEventArgs& e) override;

	private:
		std::vector<ObjectInfo> _objects;
		bool _update_required;

		void _addObject(std::shared_ptr<IObject>&& object);
		bool _removeObject(IObject::object_id id);
		bool _removeObjectIf(const std::function<bool(const ObjectInfo&)>& func);
		void _onUpdate(ApplicationContext context);

	};

}