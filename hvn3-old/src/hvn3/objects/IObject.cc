#include "hvn3/objects/IObject.h"

namespace hvn3 {

	// IObject::AnimationEndEventArgs

	IObject::AnimationEndEventArgs::AnimationEndEventArgs(int endIndex) :
		_end_index(endIndex) {}
	int IObject::AnimationEndEventArgs::EndIndex() const {

		return _end_index;

	}

	// IObject

	const IObject::object_indexer::index_type IObject::NULL_OBJECT_ID = IObject::object_indexer::NULL_INDEX;

}