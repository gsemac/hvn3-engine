#include "hvn3/objects/IObject.h"

namespace hvn3 {

	IObject::AnimationEndEventArgs::AnimationEndEventArgs(int endIndex) :
		_end_index(endIndex) {}
	int IObject::AnimationEndEventArgs::EndIndex() const {

		return _end_index;

	}

}