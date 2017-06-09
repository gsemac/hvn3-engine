#include "RoomTransitionNone.h"
#include <iostream>

namespace hvn3 {

	RoomTransitionNone::RoomTransitionNone() {}
	RoomTransitionNone::~RoomTransitionNone() {

		std::cout << "DESTRUCTED\n";

	}

	void RoomTransitionNone::ExitBegin() {}
	bool RoomTransitionNone::ExitStep(UpdateEventArgs& e) {

		return true;

	}
	bool RoomTransitionNone::EnterStep(UpdateEventArgs& e) {

		return true;

	}
	void RoomTransitionNone::OnDraw(DrawEventArgs& e) {}

}