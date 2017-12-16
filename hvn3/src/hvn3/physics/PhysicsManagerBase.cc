#include "hvn3/physics/PhysicsManagerBase.h"
#include "hvn3/utility/Algorithm.h"

namespace hvn3 {
	namespace Physics {

		PhysicsManagerBase::PhysicsManagerBase() {

		}

		void PhysicsManagerBase::AddBody(IPhysicsBody& body) {

			_bodies.push_back(&body);

		}
		void PhysicsManagerBase::RemoveBody(IPhysicsBody& body) {

			_pending_removal.push_back(&body);

		}
		const Vector2d& PhysicsManagerBase::Gravity() const {

			return _gravity;

		}
		void PhysicsManagerBase::SetGravity(const Vector2d& value) {

			_gravity = value;

		}
		size_t PhysicsManagerBase::Count() const {

			return _bodies.size();

		}
		void PhysicsManagerBase::Clear() {

			_bodies.clear();

		}

		void PhysicsManagerBase::OnUpdate(UpdateEventArgs& e) {

			// Handle any pending removals.
			if (_pending_removal.size() > 0) {
				_bodies.erase(RemoveSame(_bodies.begin(), _bodies.end(), _pending_removal.begin(), _pending_removal.end()).first, _bodies.end());
				_pending_removal.clear();
			}

		}

	}
}