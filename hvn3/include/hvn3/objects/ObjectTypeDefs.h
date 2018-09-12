#pragma once
#include "hvn3/core/SmartPointerToInterfaceWrapper.h"
#include "hvn3/utility/BitFlags.h"
#include <memory>

namespace hvn3 {

	class IObject;
	class Object;

	typedef std::shared_ptr<IObject> IObjectPtr;
	template <typename ObjectType>
	using ObjectPtr = hvn3::system::SmartPointerToInterfaceWrapper<IObjectPtr, ObjectType>;
	typedef int ObjectId;

	enum : ObjectId {
		Self = -1,
		Other = -2,
		All = -3,
		NoOne = -4
	};

	enum class ObjectFlags {
		Solid = 1,
		NoCollisions = 2,
		EnablePhysics = 4
	};
	ENABLE_BITFLAG_OPERATORS(ObjectFlags)

		struct SystemObjects {
		enum : ObjectId {
			// Object ID for helper object "Camera2d" (Camera2d.h).
			CAMERA_2D = -999
		};
	};

	template<typename ObjectType = Object, typename... Args>
	ObjectPtr<ObjectType> make_object(Args&&... args) {

		IObjectPtr smart_ptr = std::make_shared<ObjectType>(std::forward<Args>(args)...);
		ObjectPtr<ObjectType> handle(std::move(smart_ptr));

		return handle;

	}

}