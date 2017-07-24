#pragma once
#include "EventArgs.h"

namespace hvn3 {

	class CreateEventArgs : public System::EventArgs {

	public:
		CreateEventArgs() = default;

	};

	class DestroyEventArgs : public System::EventArgs {

	public:
		DestroyEventArgs() = default;

	};

}