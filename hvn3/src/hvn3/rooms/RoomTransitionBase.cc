#include "hvn3/rooms/RoomTransitionBase.h"

namespace hvn3 {

	void RoomTransitionBase::ExitEnd() {}
	void RoomTransitionBase::EnterBegin() {}
	void RoomTransitionBase::EnterEnd() {}
	bool RoomTransitionBase::Blocking() const {
		return true;
	}

}