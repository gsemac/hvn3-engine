#pragma once
#include "hvn3/utility/BitFlags.h"
#include <memory>

namespace hvn3 {

	class Object;

	typedef std::shared_ptr<IObject> ObjectPtr;
	typedef std::shared_ptr<const IObject> ConstObjectPtr;
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

}