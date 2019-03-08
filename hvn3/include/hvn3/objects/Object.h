#pragma once
#include "hvn3/assets/ImprovedAssetManager.h"
#include "hvn3/collision/CollisionDefs.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/objects/ObjectBase.h"
#include "hvn3/sprites/Sprite.h"
#include "hvn3/sprites/SpriteRenderer.h"

namespace hvn3 {

	class Object :
		public ObjectBase {

	public:
		Object();
		Object(float x, float y);
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
		void OnContextChanged(ContextChangedEventArgs& e) override;

	protected:
		virtual void OnAnimationEnd(AnimationEndEventArgs& e);

		class ApplicationContext& GetContext();
		const class ApplicationContext& GetContext() const;

		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& other);
		void SetVelocity(float x, float y);
		void AddVelocity(const Vector2d& other);
		void AddVelocity(float x, float y);

		const Sprite& Sprite() const;
		void SetSprite(const class Sprite& value);
		void SetSprite(Preview::AssetId value);

		SpriteRenderer& GetRenderer();
		const SpriteRenderer& Renderer() const;
		ICollider& GetCollider();

		template<typename T>
		void SetHitMask(const T& mask) {

			GetCollider().SetHitMask(mask);

		}

		bool PlaceFree();
		bool PlaceMeeting(int category);

	private:
		class ApplicationContext _context;
		Preview::AssetId _sprite_id;
		SpriteRenderer _sprite_renderer;
		Vector2d _velocity;
		ColliderPtr _collider;

	};

}