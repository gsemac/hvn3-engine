#pragma once
#include "hvn3/collision/CollisionFilter.h"
#include "hvn3/collision/CollisionResult.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/collision/ICollider.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/collision/IBroadPhase.h"
#include "hvn3/collision/NarrowPhase.h"
#include "hvn3/math/GeometryUtils.h"

#include <cassert>
#include <memory>
#include <list>
#include <vector>

namespace hvn3 {

	class CollisionManagerBase :
		public ICollisionManager {

	public:
		using condition_lambda_type = ICollisionManager::condition_lambda_type;

		CollisionManagerBase(std::unique_ptr<IBroadPhase>&& broad) :
			CollisionManagerBase(std::move(broad), std::make_unique<NarrowPhase>()) {
		}
		CollisionManagerBase(std::unique_ptr<IBroadPhase>&& broad, std::unique_ptr<INarrowPhase>&& narrow) :
			_broad_phase(std::move(broad)),
			_narrow_phase(std::move(narrow)) {

			// Use default precision of 0.4, precise to 2/5ths of a pixel. Anything less may produce visually-noticeable inaccuracies.
			_precision = 0.4f;

		}
		~CollisionManagerBase() {
			_broad_phase->Clear();
		}

		void Add(const IColliderPtr& collider) override {
			_broad_phase->Add(collider);
		}
		const IBroadPhase& Broad() const override {

			assert(static_cast<bool>(_broad_phase));

			return *_broad_phase;

		}
		const INarrowPhase& Narrow() const override {

			assert(static_cast<bool>(_narrow_phase));

			return *_narrow_phase;

		}
		const std::vector<CollisionResult>& CollidingPairs() const override {
			return _pair_buffer;
		}
		size_t Count() const override {
			return _broad_phase->Count();
		}
		void Clear() override {
			_broad_phase->Clear();
		}

		float Precision() const {
			return _precision;
		}
		void SetPrecision(float value) {
			_precision = value;
		}

		void OnUpdate(UpdateEventArgs& e) override {

			// Update the state of the collision detection method.
			_broad_phase->OnUpdate(e);

			// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
			CheckPairs(_broad_phase->FindCandidatePairs());

		}

		bool PlaceFree(ICollider& body) override {
			return PlaceFree(body, body.Position());
		}
		bool PlaceFree(ICollider& body, CollisionResult& manifold) override {
			return PlaceFree(body, body.Position(), manifold);
		}
		bool PlaceFree(ICollider& body, const PointF& position) override {

			CollisionResult dummy;

			return PlaceFree(body, position, dummy);

		}
		bool PlaceFree(ICollider& body, const PointF& position, CollisionResult& manifold) override {
			return !PlaceMeeting(body, position, [](const ICollider*) { return true; }, manifold);
		}

		bool PlaceMeeting(ICollider& body, int category) override {
			return PlaceMeeting(body, body.Position(), category);
		}
		bool PlaceMeeting(ICollider& body, int category, CollisionResult& manifold) override {
			return PlaceMeeting(body, body.Position(), category, manifold);
		}
		bool PlaceMeeting(ICollider& body, const PointF& position, int category) override {

			CollisionResult dummy;

			return PlaceMeeting(body, position, category, dummy);

		}
		bool PlaceMeeting(ICollider& body, const PointF& position, int category, CollisionResult& manifold) override {
			return PlaceMeeting(body, position, [=](const ICollider* body) { return (body->Filter().CategoryBits() & category) != 0; }, manifold);
		}
		bool PlaceMeeting(ICollider& body, const PointF& position, const condition_lambda_type& where, CollisionResult& manifold) override {

			// If the object does not have a collision mask, return false immediately (no collisions are possible).
			if (!body.HitMask())
				return false;

			// Create a vector to store the results.
			IBroadPhase::collider_vector_type hits;

			// Get a list of all colliders that could potentially collide with the collider.
			RectangleF aabb = body.AABB().Translate(-body.X(), -body.Y()).Translate(position.X(), position.Y());
			Broad().QueryRegion(aabb, hits, body.Filter().MaskBits());

			// If the list is empty, the place is free.
			if (hits.size() == 0)
				return false;

			for (size_t i = 0; i < hits.size(); ++i) {

				// Ignore self and objects that don't meet the given condition.
				if (hits[i] == &body || !where(hits[i]))
					continue;

				// Check for a collision.
				if (Narrow().TestCollision(&body, position, hits[i], hits[i]->Position(), manifold)) {
					manifold.collider = hits[i];
					return true;
				}

			}

			// The collider did not collide with any other colliders, so the place is free.
			return false;

		}

		bool MoveContact(ICollider* body, float direction, float distance) override {

			return MoveContactIf(body, direction, distance, [](const ICollider*) { return true; });

		}
		bool MoveContact(ICollider* body, float direction, float distance, int category) override {

			return MoveContactIf(body, direction, distance, [=](const ICollider* body) { return (body->Filter().CategoryBits() & category) != 0; });

		}
		bool MoveContactIf(ICollider* body, float direction, float distance, const condition_lambda_type& condition) override {

			CollisionResult manifold;

			return MoveContactIf(body, direction, distance, manifold, condition);

		}
		bool MoveContactIf(ICollider* body, float direction, float distance, CollisionResult& manifold, const condition_lambda_type& condition) override {

			// If the distance is negative, reverse the direction and then make it positive.
			if (distance < 0.0f) {
				direction += 180.0f;
				distance = Math::Abs(distance);
			}

			// If the distance is 0, just return if the current position is free.
			if (Math::IsZero(distance))
				return PlaceMeeting(*body, body->Position(), condition, manifold);

			float distance_per_step = Math::Min(body->AABB().Width(), body->AABB().Height(), distance);

			if (distance_per_step <= 0.0f) {
				// The body doesn't have a valid AABB, and thus will never collide with anything. Just move to the position and return false to indicate no collisions occurred.
				body->SetPosition(Math::Geometry::PointInDirection(body->Position(), direction, distance));
				return false;
			}

			float distance_total = 0.0f;
			PointF new_position = body->Position();
			bool place_free;
			bool contact_made = false;

			while (distance_total < (std::abs)(distance)) {

				PointF tentative_position = Math::Geometry::PointInDirection(new_position, direction, distance_per_step);
				place_free = !PlaceMeeting(*body, tentative_position, condition, manifold);

				// If we make contact with an object, start halving the movement distance so we can get close enough to satisfy the precision value.
				if (!place_free) {
					contact_made = true;
					if (distance_per_step < _precision)
						break;
					else {
						distance_per_step /= 2.0f;
						continue;
					}
				}

				new_position = tentative_position;
				distance_total += distance_per_step;

				// If we know that there's going to be a collision, there is no point in trying to move the same half again. Half the movement distance immediately.
				if (contact_made)
					distance_per_step /= 2.0f;

				// If the distance we're now moving is below the precision threshold, there is no need to continue.
				if (distance_per_step < _precision)
					break;

			}

			body->SetPosition(new_position);

			return contact_made;

		}
		bool MoveOutside(ICollider* body, float direction, float max_distance) override {

			PointF pos = body->Position();
			float dist = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			bool place_free;

			// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
			while ((place_free = PlaceFree(*body, pos), !place_free) && dist < max_distance) {

				pos = Math::Geometry::PointInDirection(pos, direction, distance_per_step);

				dist += distance_per_step;

			}

			body->SetPosition(pos);

			return place_free;

		}
		bool MoveOutsideBody(ICollider* body, ICollider* other, float direction, float max_distance) override {

			float dist = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			bool place_free;
			CollisionResult m;

			// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
			while ((place_free = Narrow().TestCollision(body, other, m), place_free) && dist < (std::abs)(max_distance)) {

				body->SetPosition(Math::Geometry::PointInDirection(body->Position(), direction, distance_per_step));

				dist += distance_per_step;

			}

			return place_free;

		}

	protected:
		// Returns the vector of colliding pairs from the last update.
		std::vector<CollisionResult>& GetPairs() {

			return _pair_buffer;

		}
		// Returns the vector of colliding pairs from the last update.
		const std::vector<CollisionResult>& Pairs() const {

			return _pair_buffer;

		}
		// Checks all potentially-colliding pairs and triggers events as needed.
		virtual void CheckPairs(const IBroadPhase::collider_pair_vector_type& pairs) = 0;

	private:
		std::vector<CollisionResult> _pair_buffer;
		std::unique_ptr<IBroadPhase> _broad_phase;
		std::unique_ptr<INarrowPhase> _narrow_phase;
		float _precision;

	};

}