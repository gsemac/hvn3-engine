#pragma once
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class ViewUpdateEventArgs : public UpdateEventArgs {

	public:
		ViewUpdateEventArgs(double dt, Size<int> room_size);
		const Size<int> RoomSize() const;

	private:
		Size<int> _room_size;

	};

}