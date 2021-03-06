#pragma once

#include "hvn3/objects/IObject.h"

#include <limits>

namespace hvn3 {

	class ObjectBase :
		public IObject {

		typedef int depth_type;

	public:
		static const depth_type MIN_DEPTH = std::numeric_limits<depth_type>::min();
		static const depth_type MAX_DEPTH = std::numeric_limits<depth_type>::max();

		ObjectBase(ObjectId id);
		ObjectBase(ObjectId id, const PointF& position);
		ObjectBase(ObjectId id, float x, float y);
		ObjectBase(ObjectId id, ObjectFlags flags);
		ObjectBase(ObjectId id, const PointF& position, ObjectFlags flags);
		ObjectBase(ObjectId id, float x, float y, ObjectFlags flags);

		void OnCreate(CreateEventArgs& e) override;
		void OnDestroy(DestroyEventArgs& e) override;
		void OnContextChanged(ContextChangedEventArgs& e) override;

		ObjectId Id() const override;
		depth_type Depth() const override;
		void SetDepth(depth_type depth) override;
		bool IsDestroyed() const override;
		void Destroy() override;
		bool IsActive() const override;
		void SetActive(bool value) override;
		ObjectFlags Flags() const override;
		void SetFlags(ObjectFlags flags) override;

		void OnDraw(DrawEventArgs& e) override;
		void OnUpdate(UpdateEventArgs& e) override;

		void OnCollide(CollisionEventArgs& e) override;

		float X() const override;
		float Y() const override;
		void SetX(float x) override;
		void SetY(float y) override;
		PointF Position() const override;
		void SetPosition(const PointF& position) override;
		void SetPosition(float x, float y) override;

	private:
		ObjectId _id;
		PointF _position;
		int _depth;
		bool _is_destroyed;
		bool _is_active;
		ObjectFlags _flags;

	};

}