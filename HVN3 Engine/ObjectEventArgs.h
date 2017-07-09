#pragma once
#include "EventArgs.h"

namespace hvn3 {

	class CreateEventArgs : public EventArgs {

	public:
		CreateEventArgs() = default;

	};

	class DestroyEventArgs : public EventArgs {

	public:
		DestroyEventArgs() = default;

	};

}