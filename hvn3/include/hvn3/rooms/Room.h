#pragma once
#include "hvn3/rooms/RoomBase.h"
#include "hvn3/collision/CollisionManager.h"
#include "hvn3/collision/SpacialPartitioningGrid.h"
#include "hvn3/collision/NarrowPhase.h"
#include "hvn3/backgrounds/BackgroundManager.h"
#include "hvn3/views/ViewManager.h"
#include "hvn3/physics/BasicPhysicsManager.h"
#include "hvn3/objects/ObjectManager.h"
#include <memory>

#pragma warning(push)
#pragma warning(disable:4250)

namespace hvn3 {

	class Room : public RoomBase<ObjectManager> {

	public:
		typedef ObjectCollisionManager<SpacialPartitioningGrid<32, 32>, NarrowPhase> collision_manager_type;

		Room(RoomId id, const SizeI& size);
		virtual ~Room() = default;

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

		const View* CurrentView() const override;

		const IBackgroundManager& Backgrounds() const override;
		IBackgroundManager& Backgrounds() override;
		const IViewManager& Views() const override;
		IViewManager& Views() override;
		const collision_manager_type& Collisions() const override;
		collision_manager_type& Collisions() override;
		virtual const Physics::IPhysicsManager& Physics() const override;
		virtual Physics::IPhysicsManager& Physics() override;

		virtual RectangleF GetVisibleRegion() override;

		virtual void Restart() override;

		void SetContext(hvn3::Context context) override;

	protected:
		virtual void OnReset() override;
		virtual void OnRender(DrawEventArgs& e) override;

	private:
		collision_manager_type  _collision_manager;
		hvn3::BackgroundManager _background_manager;
		hvn3::ViewManager _view_manager;
		std::unique_ptr<hvn3::Physics::BasicPhysicsManager> _physics_manager;

		size_t _rendering_view;
		bool _restart_pending;

	};

}