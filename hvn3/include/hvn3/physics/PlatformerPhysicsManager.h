#pragma once
#include "hvn3/core/ApplicationContext.h"
#include "hvn3/physics/PhysicsBodyBase.h"
#include "hvn3/physics/PhysicsManagerBase.h"

#include <unordered_map>

namespace hvn3 {

	class PlatformerPhysicsManager :
		public Physics::PhysicsManagerBase {

	public:
		void OnUpdate(UpdateEventArgs& e) override;

	protected:
		void OnContextChanged(ContextChangedEventArgs& e) override;

	private:
		ApplicationContext _context;

	};

}