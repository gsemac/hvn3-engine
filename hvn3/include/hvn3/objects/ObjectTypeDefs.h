#pragma once
#include "hvn3/utility/BitFlags.h"
#include <memory>

namespace hvn3 {

	class IObject;
	class Object;

	typedef std::shared_ptr<IObject> ObjectPtr;
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

	template<typename object_type = Object, typename... Args>
	ObjectPtr make_object(Args&&... args) {

		ObjectPtr ptr = std::make_shared<object_type>(std::forward<Args>(args)...);

		return ptr;

	}

}