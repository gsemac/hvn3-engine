#pragma once
#include "hvn3/utility/BitFlags.h"
#include <memory>

namespace hvn3 {

	class Object;

	typedef std::shared_ptr<Object> ObjectPtr;
	typedef std::shared_ptr<const Object> ConstObjectPtr;
	typedef int ObjectId;
	
	enum : ObjectId {
		Self = -1,
		Other = -2,
		All = -3,
		NoOne = -4
	};

	enum class ObjectFlags {
		Solid = 1,
		NoCollision = 2,
		NoPhysics = 4
	};
	ENABLE_BITFLAG_OPERATORS(ObjectFlags)

}