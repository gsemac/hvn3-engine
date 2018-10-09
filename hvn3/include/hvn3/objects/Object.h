#pragma once
#include "hvn3/math/Vector2d.h"
#include "hvn3/objects/ObjectBase.h"

namespace hvn3 {

	class Object :
		public ObjectBase {

	public:
		Object();
		Object(ObjectId id);
		Object(ObjectId id, const PointF& position);
		Object(ObjectId id, float x, float y);
		Object(ObjectId id, ObjectFlags flags);
		Object(ObjectId id, const PointF& position, ObjectFlags flags);
		Object(ObjectId id, float x, float y, ObjectFlags flags);
		~Object();

		void OnCreate(CreateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;
		void OnUpdate(UpdateEventArgs& e) override;
		void OnDestroy(DestroyEventArgs& e) override;

		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& other);
		void SetVelocity(float x, float y);
		void AddVelocity(const Vector2d& other);
		void AddVelocity(float x, float y);

	private:
		Vector2d _velocity;

	};

}