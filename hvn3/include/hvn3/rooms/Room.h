#pragma once
#include "hvn3/rooms/RoomBase.h"
#include "hvn3/collision/CollisionManager.h"
#include "hvn3/collision/SpacialPartitioningGrid.h"
#include "hvn3/collision/NarrowPhase.h"
#include "hvn3/backgrounds/BackgroundManager.h"
#include "hvn3/views/ViewManager.h"
#include "hvn3/physics/BasicPhysicsManager.h"
#include "hvn3/objects/ObjectManager.h"
#include "hvn3/tilesets/TileManager.h"
#include <memory>
#include <unordered_map>

#pragma warning(push)
#pragma warning(disable:4250)

namespace hvn3 {

	class Room : public RoomBase {

	public:
		typedef BackgroundManager background_manager_type;
		typedef ObjectCollisionManager<SpacialPartitioningGrid<32, 32>, NarrowPhase> collision_manager_type;
		typedef ObjectManager object_manager_type;
		typedef hvn3::Physics::BasicPhysicsManager physics_manager_type;
		typedef TileManager tile_manager_type;
		typedef ViewManager view_manager_type;

		Room(RoomId id, const SizeI& size);
		Room(RoomId id, int width, int height);
		Room(const SizeI& size);
		Room(int width, int height);

		virtual ~Room() = default;

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

		IObjectManager& GetObjects() override;
		IBackgroundManager& GetBackgrounds() override;
		IViewManager& GetViews() override;
		collision_manager_type& GetCollisions() override;
		Physics::IPhysicsManager& GetPhysics() override;
		TileManager& GetTiles() override;

		const IObjectManager& Objects() const override;
		const IBackgroundManager& Backgrounds() const override;
		const IViewManager& Views() const override;
		const collision_manager_type& Collisions() const override;
		const Physics::IPhysicsManager& Physics() const override;
		const TileManager& Tiles() const override;

		void SetBackground(const Background& value) override;

		const View& CurrentView() const override;
		RectangleF VisiblePort() const override;
		RectangleF VisibleRegion() const override;

	protected:
		void SetContext(hvn3::Context context) override;

		void Restart() override;
		void OnReset() override;
		void OnRender(DrawEventArgs& e) override;

	private:
		void _initializeAllManagers();

		// #todo Perhaps all managers should be stored as pointers to allow them to be swapped easily.
		collision_manager_type  _collision_manager;
		background_manager_type _background_manager;
		view_manager_type _view_manager;
		std::unique_ptr<tile_manager_type> _tile_manager;
		std::unique_ptr<physics_manager_type> _physics_manager;
		std::unique_ptr<object_manager_type> _object_manager;

		size_t _rendering_view;
		bool _restart_pending;

	};

}