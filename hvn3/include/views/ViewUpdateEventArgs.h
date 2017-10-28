#pragma once
#include "core/UpdateEventArgs.h"
#include "utility/Size.h"

namespace hvn3 {

	class ViewUpdateEventArgs : public UpdateEventArgs {

	public:
		ViewUpdateEventArgs(float dt, Size<int> room_size);
		const Size<int> RoomSize() const;

	private:
		Size<int> _room_size;

	};

}