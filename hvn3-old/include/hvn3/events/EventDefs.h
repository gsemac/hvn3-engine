#pragma once

#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/events/UpdateEventArgs.h"
#include "hvn3/utility/TypeList.h"

namespace hvn3 {
	namespace events {

		using UpdateEvents = TypeList<UpdateBeginEventArgs, UpdateEventArgs, UpdateEndEventArgs>;
		using DrawEvents = TypeList<DrawEventArgs>;

	}
}