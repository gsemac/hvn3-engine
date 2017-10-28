#include "rooms/IRoomTransition.h"

namespace hvn3 {

	void IRoomTransition::ExitEnd() {}

	void IRoomTransition::EnterBegin() {}
	void IRoomTransition::EnterEnd() {}

	bool IRoomTransition::AllowRoomUpdate() const {

		return false;

	}

}