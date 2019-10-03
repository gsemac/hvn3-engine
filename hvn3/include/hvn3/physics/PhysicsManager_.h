#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/math/Vector2d.h"

namespace hvn3 {

	class PhysicsManager_ :
		public ManagerBase<>,
		public EventListenerBase<events::UpdateEvents> {

	public:
		PhysicsManager_();

		void SetGravity(float value);
		void SetGravity(float x, float y);
		void SetGravity(const Vector2d& vector);

		void OnStart(StartEventArgs& e) override;
		void OnEnd(EndEventArgs& e) override;
		void OnEvent(UpdateEventArgs& e) override;

	private:
		Vector2d _gravity;

		void _updatePositions(UpdateEventArgs& e);

	};

}