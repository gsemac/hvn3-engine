#pragma once
#include "hvn3/math/Vector2d.h"
#include "hvn3/objects/ObjectBase.h"
#include "hvn3/sprites/Sprite.h"
#include "hvn3/sprites/SpriteRenderer.h"

namespace hvn3 {

	class ICollisionBody;

	namespace Physics {
		class IPhysicsBody;
	}

	class Object : public ObjectBase {

		typedef Sprite sprite_type;

	public:
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

		const sprite_type& Sprite() const;
		sprite_type& GetSprite();
		void SetSprite(const sprite_type& other);
		void SetSprite(sprite_type&& other);
		const Graphics::SpriteRenderer& Renderer() const;
		Graphics::SpriteRenderer& GetRenderer();
		void SetRenderer(const Graphics::SpriteRenderer& other);
		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& other);
		void SetVelocity(float x, float y);
		void AddVelocity(const Vector2d& other);
		void AddVelocity(float x, float y);
		ICollisionBody* GetCollisionBody();
		const ICollisionBody* CollisionBody() const;
		Physics::IPhysicsBody* GetPhysicsBody();
		const Physics::IPhysicsBody* PhysicsBody() const;

	private:
		sprite_type _sprite;
		Graphics::SpriteRenderer _renderer;
		Vector2d _velocity;
		ICollisionBody* _collision_body;
		Physics::IPhysicsBody* _physics_body;

		void _createCollisionBody();
		void _createPhysicsBody();
		void _destroyCollisionBody();
		void _destroyPhysicsBody();

	};

}