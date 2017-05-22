#pragma once
#include "UpdateEventArgs.h"
#include "Size.h"

namespace hvn3 {

	class ViewUpdateEventArgs : public UpdateEventArgs {

	public:
		ViewUpdateEventArgs(float dt, Size room_size);
		const Size RoomSize() const;

	private:
		Size _room_size;

	};

}