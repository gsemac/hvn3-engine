#pragma once

#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/utility/TypeList.h"

namespace hvn3 {

	using UpdateEvents = TypeList<UpdateBeginEventArgs, UpdateEventArgs, UpdateEndEventArgs>;

}