#pragma once

#include "hvn3/collision/BroadphasePair.h"
#include "hvn3/collision/SpatialGrid.h"
#include "hvn3/components/ColliderComponent.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"

#include <vector>

namespace hvn3 {

	class CollisionManager_ :
		public ManagerBase<>,
		public EventListenerBase<events::UpdateEvents, events::DrawEvents> {

	public:
		CollisionManager_();

		void OnStart(StartEventArgs& e) override;
		void OnEnd(EndEventArgs& e) override;

		void OnEvent(UpdateEventArgs& e) override;
		void OnEvent(DrawEventArgs& e) override;

		void GetBroadphasePairs(std::vector<BroadphasePair>& output, bool ignoreCache = false);

		void SetDebugModeEnabled(bool value);

	private:
		ApplicationContext _context;
		SpatialGrid<BroadphasePair::value_type> _broadphase;
		bool _debug_mode_enabled;

		void _refreshBroadphase();
		void _refreshCollisions();

	};

}