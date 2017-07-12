#pragma once
#include "RoomBase.h"
#include "BackgroundManager.h"
#include "ViewManager.h"
#include "CollisionManager.h"
#include "IPhysicsManager.h"
#include "UniqueCreateableBase.h"
#include "ObjectManager.h"

namespace hvn3 {

	// This class extends the responsibilites of the room class to include background, tile, and view management.
	class Room : public RoomBase<ObjectManager> {

	public:
		Room(RoomId id, const SizeI& size);
		virtual ~Room() = default;

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

		const View* CurrentView() const override;

		const IBackgroundManager* Backgrounds() const override;
		IBackgroundManager* Backgrounds() override;
		const IViewManager* Views() const override;
		IViewManager* Views() override;
		const Collision::ICollisionManager<Object*>* Collisions() const override;
		Collision::ICollisionManager<Object*>* Collisions() override;
		virtual const Physics::IPhysicsManager<Object*>* Physics() const override;
		virtual Physics::IPhysicsManager<Object*>* Physics() override;
	
		virtual RectangleF GetVisibleRegion() override;

	protected:
		virtual void OnReset() override;
		virtual void OnRender(DrawEventArgs& e) override;

	private:
		Collision::CollisionManager _collision_manager;
		hvn3::BackgroundManager _background_manager;
		hvn3::ViewManager _view_manager;

		size_t _rendering_view;

	};

}