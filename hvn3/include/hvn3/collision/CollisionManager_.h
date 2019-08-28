#pragma once

#include "hvn3/collision/SpatialGrid.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"

namespace hvn3 {

	class CollisionManager_ :
		public ManagerBase<>,
		public EventListenerBase<events::UpdateEvents> {

	public:
		void OnStart(StartEventArgs& e) override;
		void OnEnd(EndEventArgs& e) override;

		void OnEvent(UpdateEventArgs& e) override;

	private:
		SpatialGrid< _grid;

	};

}