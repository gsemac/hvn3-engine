#pragma once
#include "hvn3/collision/CollisionManifold.h"
#include "hvn3/collision/CollisionBodyMutator.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/collision/IBroadPhase.h"
#include <vector>

namespace hvn3 {

	class ICollisionBody;

	template <typename broadphase_type, typename narrowphase_type, typename body_type>
	class CollisionManagerBase : public ICollisionManager {

	public:
		typedef broadphase_type broadphase_type;
		typedef narrowphase_type narrowphase_type;
		typedef body_type body_type;

		CollisionManagerBase() = default;
		~CollisionManagerBase() {

			// Clear all bodies from the broad phase manager, which will clear each one's manager pointer.
			_broad_phase.Clear();

			//// Remove self as the manager for all collision bodies.
			//// This is so that, when the bodies fall out of scope, they do not try to remove themselves from the manager.
			//for (auto i = Bodies().begin(); i != Bodies().end(); ++i)
			//	System::CollisionBodyMutator(*i).SetManager(nullptr);

		}

		void AddBody(ICollisionBody& body) override {

			System::CollisionBodyMutator(body).SetManager(this);

			_broad_phase.AddBody(body);

		}
		void RemoveBody(ICollisionBody& body) override {

			System::CollisionBodyMutator(body).SetManager(nullptr);

			_broad_phase.RemoveBody(body);

		}
		broadphase_type& BroadPhase() override {

			return _broad_phase;

		}
		narrowphase_type& NarrowPhase() override {

			return _narrow_phase;

		}
		const std::vector<CollisionManifold>& CollidingPairs() const override {

			return _pairs;

		}
		size_t Count() const override {

			return _broad_phase.Count();

		}
		void Clear() override {

			_broad_phase.Clear();

		}

		void OnUpdate(UpdateEventArgs& e) override {

			// Update the state of the collision detection method.
			_broad_phase.OnUpdate(e);

			// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
			CheckPairs(_broad_phase.FindCandidatePairs());

		}

		bool PlaceFree(ICollisionBody& body, const PointF& position) override {

			return PlaceFreeIf(body, position, [](ICollisionBody*) { return true; });

		}
		bool PlaceFree(ICollisionBody& body, float x, float y) override {

			return PlaceFree(body, PointF(x, y));

		}
		bool PlaceFreeIf(ICollisionBody& body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition) override {

			CollisionManifold m;

			return PlaceFreeIf(body, position, condition, m);

		}
		bool PlaceFreeIf(ICollisionBody& body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition, CollisionManifold& manifold) override {

			// If the object does not have a collision mask, return true immediately (no collisions are possible).
			if (body.HitMask() == nullptr)
				return true;

			// Create a vector to store the results.
			IBroadPhase::collider_vector_type hits;

			// Get a list of all colliders that could potentially collide with the collider.
			RectangleF aabb = body.AABB();
			aabb.Translate(-body.X(), -body.Y());
			aabb.Translate(position.X(), position.Y());
			BroadPhase().QueryRegion(aabb, hits, body.Category().MaskBits());

			// If the list is empty, the place is free.
			if (hits.size() == 0)
				return true;

			for (size_t i = 0; i < hits.size(); ++i) {

				// Ignore self and objects that don't meet the given condition.
				if (hits[i] == &body || !condition(hits[i]))
					continue;

				// Check for a collision.
				if (NarrowPhase().TestCollision(body, position, *hits[i], hits[i]->Position(), manifold))
					return false;

			}

			// The collider did not collide with any other colliders, so the place is free.
			return true;

		}
		bool MoveContact(ICollisionBody& body, float direction, float max_distance) override {

			return MoveContactIf(body, direction, max_distance, [](ICollisionBody*) { return true; });

		}
		bool MoveContactIf(ICollisionBody& body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) override {

			// If the distance is negative, reverse the direction and then make it positive.
			if (max_distance < 0.0f) {
				direction += 180.0f;
				max_distance = Math::Abs(max_distance);
			}

			// If the distance is 0, just return if the current position is free.
			if (Math::IsZero(max_distance))
				return PlaceFreeIf(body, body.Position(), condition);

			float distance = 0.0f;
			float distance_per_step = Math::Min(body.AABB().Width(), body.AABB().Height(), max_distance);
			PointF new_position = body.Position();
			bool place_free;

			if (distance_per_step <= 0.0f) {
				// The body doesn't have a valid AABB, and thus will never collide with anything. Just move to the position and return false to indicate no collisions occurred.
				body.SetPosition(Math::Geometry::PointInDirection(body.Position(), direction, max_distance));
				return false;
			}

			while (distance < (std::abs)(max_distance)) {

				PointF tentative_position = Math::Geometry::PointInDirection(new_position, direction, distance_per_step);
				place_free = PlaceFreeIf(body, tentative_position, condition);

				if (!place_free) {
					if (distance_per_step < 0.2f)
						break;
					else {
						distance_per_step /= 2.0f;
						continue;
					}
				}

				new_position = tentative_position;
				distance += distance_per_step;

			}

			body.SetPosition(new_position);

			return !place_free;

		}
		bool MoveOutside(ICollisionBody& body, float direction, float max_distance) override {

			PointF pos = body.Position();
			float dist = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			bool place_free;

			// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
			while ((place_free = PlaceFree(body, pos), !place_free) && dist < max_distance) {

				pos = Math::Geometry::PointInDirection(pos, direction, distance_per_step);

				dist += distance_per_step;

			}

			body.SetPosition(pos);

			return place_free;

		}
		bool MoveOutsideBody(ICollisionBody& body, ICollisionBody& other, float direction, float max_distance) override {

			float dist = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			bool place_free;
			CollisionManifold m;

			// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
			while ((place_free = NarrowPhase().TestCollision(body, other, m), place_free) && dist < (std::abs)(max_distance)) {

				body.SetPosition(Math::Geometry::PointInDirection(body.Position(), direction, distance_per_step));

				dist += distance_per_step;

			}

			return place_free;

		}

	protected:
		// Returns the vector of colliding pairs from the last update.
		std::vector<CollisionManifold>& Pairs() {

			return _pairs;

		}
		// Returns the vector of colliding pairs from the last update.
		const std::vector<CollisionManifold>& Pairs() const {

			return _pairs;

		}
		// Checks all potentially-colliding pairs and triggers events as needed.
		virtual void CheckPairs(const typename broadphase_type::collider_pair_vector_type& pairs) = 0;

	private:
		std::vector<CollisionManifold> _pairs;
		broadphase_type _broad_phase;
		narrowphase_type _narrow_phase;

	};

}