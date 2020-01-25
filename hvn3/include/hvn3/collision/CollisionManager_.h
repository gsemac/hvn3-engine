#pragma once

#include "hvn3/collision/BroadphasePair.h"
#include "hvn3/collision/SpatialGrid.h"
#include "hvn3/components/ColliderComponent.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/IEventManager.h"

#include <vector>

namespace hvn3 {

	class CollisionManager_ :
		public ManagerBase<>,
		public EventListenerBase<events::UpdateEvents, events::DrawEvents> {

	public:
		using pair_buffer_type = std::vector<BroadphasePair>;

		CollisionManager_(IEventManager* eventManager, ecs::ComponentManager* componentManager);

		~CollisionManager_();

		void OnStart(StartEventArgs& e) override;
		void OnEnd(EndEventArgs& e) override;

		void OnEvent(UpdateEventArgs& e) override;
		void OnEvent(DrawEventArgs& e) override;

		const pair_buffer_type& GetBroadphasePairs(bool ignoreCache = false);
		const pair_buffer_type& GetCollidingPairs(bool ignoreCache = false);

		void SetDebugModeEnabled(bool value);

	private:
		IEventManager* eventManager;
		ecs::ComponentManager* componentManager;
		SpatialGrid<BroadphasePair::value_type> _broadphase;
		pair_buffer_type _pair_buffer;
		bool _debug_mode_enabled;

		void _refreshBroadphase();
		void _refreshCollisions();

	};

}