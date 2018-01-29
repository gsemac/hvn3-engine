#pragma once
#include "hvn3/collision/CollisionTypeDefs.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/objects/ObjectEventArgs.h"
#include "hvn3/objects/ObjectTypeDefs.h"
#include "hvn3/physics/PhysicsTypeDefs.h"
#include "hvn3/sprites/SpriteRenderer.h"

namespace hvn3 {

	class IObject :
		public IUpdatable,
		public IDrawable,
		public IPositionable2d<float> {

	public:
		~IObject() = default;

		virtual void OnCreate(CreateEventArgs& e) = 0;
		virtual void OnDestroy(DestroyEventArgs& e) = 0;

		virtual ObjectId Id() const = 0;
		virtual int Depth() const = 0;
		virtual void SetDepth(int depth) = 0;
		virtual bool IsDestroyed() const = 0;
		virtual void Destroy() = 0;
		virtual bool IsActive() const = 0;
		virtual void SetActive(bool value) = 0;
		virtual ObjectFlags Flags() const = 0;
		virtual void SetFlags(ObjectFlags flags) = 0;

		virtual CollisionBodyPtr& CollisionBody() = 0;
		virtual const CollisionBodyPtr& CollisionBody() const = 0;
		virtual PhysicsBodyPtr& PhysicsBody() = 0;
		virtual const PhysicsBodyPtr& PhysicsBody() const = 0;

		virtual const hvn3::Sprite& Sprite() const = 0;
		virtual Graphics::SpriteRenderer& Renderer() = 0;
		virtual const Graphics::SpriteRenderer& Renderer() const = 0;

	};

}