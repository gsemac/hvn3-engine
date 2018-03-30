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
#include <unordered_map>

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

		IBackgroundManager& GetBackgrounds() override;
		IViewManager& GetViews() override;
		collision_manager_type& GetCollisions() override;
		Physics::IPhysicsManager& GetPhysics() override;
		const IBackgroundManager& Backgrounds() const override;
		const IViewManager& Views() const override;
		const collision_manager_type& Collisions() const override;
		const Physics::IPhysicsManager& Physics() const override;

		void SetBackground(const Background& value) override {
			GetBackgrounds().Clear();
			GetBackgrounds().Add(value);
		}

		const View& CurrentView() const override;
		RectangleF VisiblePort() const override;
		RectangleF VisibleRegion() const override;

		void SetContext(hvn3::Context context) override;

		System::ManagerBase& GetManagerById(System::ManagerId id) override;

		void Restart() override;

	protected:
		void OnReset() override;
		void OnRender(DrawEventArgs& e) override;

	private:
		void _addManager(System::ManagerId id, System::ManagerBase* manager) override;

		collision_manager_type  _collision_manager;
		hvn3::BackgroundManager _background_manager;
		hvn3::ViewManager _view_manager;
		std::unique_ptr<hvn3::Physics::BasicPhysicsManager> _physics_manager;
		std::unordered_map<System::ManagerId, System::ManagerBase*> _registered_managers;

		size_t _rendering_view;
		bool _restart_pending;

	};

}