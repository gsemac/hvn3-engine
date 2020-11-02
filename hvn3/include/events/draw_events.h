#pragma once

#include "core/type_list.h"
#include "events/event_base.h"

namespace hvn3::events {

	class DrawFrameEvent :
		EventBase<> {
	};

	class DrawEvent :
		EventBase<> {
	};

	using DrawEvents = core::TypeList<DrawFrameEvent, DrawEvent>;

}