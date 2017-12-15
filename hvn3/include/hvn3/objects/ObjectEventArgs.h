#pragma once
#include "hvn3/core/GameStateEventArgs.h"

namespace hvn3 {

	class CreateEventArgs : public System::GameStateEventArgs {

	public:
		using GameStateEventArgs::GameStateEventArgs;

	};

	class DestroyEventArgs : public System::GameStateEventArgs {

	public:
		using GameStateEventArgs::GameStateEventArgs;

	};

}