#pragma once

#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/utility/TypeList.h"

namespace hvn3 {
	namespace events {

		using UpdateEvents = TypeList<UpdateBeginEventArgs, UpdateEventArgs, UpdateEndEventArgs>;

	}
}